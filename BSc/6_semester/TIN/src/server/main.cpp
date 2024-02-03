/*
 * Project: TinyAd
 * Description: Content management (server) implementation
 * Authors: Damian Kolaska
 *          Rafał Kulus
 *          Kamil Przybyła
 * Created: 03.05.2021
 */

#include <netinet/in.h>

#include <atomic>
#include <mutex>

#include "cli.hpp"
#include "common.hpp"
#include "message_deserializer.hpp"
#include "message_queue.hpp"
#include "network_utils.hpp"
#include "schedule.hpp"
#include "server/utils.hpp"

using namespace TinyAd;

//
// GLOBALS
//

WeakUdpConnection conn(0.1, SRV_PORT);

// MessageQueue has built-in sync, no need for mutex
MessageQueue msgq(4000);

std::mutex fss_m;
FileSerializerMap fss;

std::mutex pi_m;
PanelInfoMap pi;

std::atomic<bool> should_close = false;

const std::string SERVER_LOG = "logs/server.log";

ServerContext serverContext{getTimeInMicroseconds(), getTimeInMicroseconds()};

//
// CLI
//

void handleSchedule(const CLI::Command& command) {
    if (!command.value) {
        throw std::runtime_error("Expected schedule file name!");
    }

    std::string sched_path = command.value.value();

    {
        // Invalidate old files, only files from new schedule should be available
        std::lock_guard fss_lock(fss_m);
        fss.clear();
    }

    std::cout << "Sending schedule file " << sched_path << "...\n";

    const Schedule sched(sched_path);
    for (const auto& entry : sched.getEntries()) {
        std::lock_guard fss_lock(fss_m);
        sendFile(conn, fss, entry.getFilename(), serverContext);
    }

    std::lock_guard fss_lock(fss_m);
    sendFile(conn, fss, sched_path, serverContext);
}

void handlePanic(const CLI::Command& command) {
    if (!command.value) {
        throw std::runtime_error("Expected content file name!");
    }

    const auto name_it = command.args.find("name");
    const auto tags_it = command.args.find("tags");

    if (name_it != command.args.end()) {
        const auto panel_name = name_it->second;
        std::cout << "Sending important content file " << command.value.value()
                  << " to panel named " << panel_name << std::endl;

        std::lock_guard pi_lock(pi_m);
        std::lock_guard fss_lock(fss_m);
        panicSendByPanelName(conn, fss, serverContext, command.value.value(), pi, panel_name);
    } else if (tags_it != command.args.end()) {
        std::cout << "Sending important content file " << command.value.value()
                  << " to panels with tags " << tags_it->second << std::endl;

        const auto tags = ConfigValue(tags_it->second).parseList();
        std::lock_guard pi_lock(pi_m);
        std::lock_guard fss_lock(fss_m);
        panicSendByTags(conn, fss, serverContext, command.value.value(), pi, tags);
    } else {
        throw std::runtime_error("Expected --tags or --name arguments!");
    }
}

void handleTop(const CLI::Command& command) {
    if (!command.value) {
        throw std::runtime_error("Expected number of log lines to display!");
    }

    try {
        const auto n = std::stoul(command.value.value());
        std::cout << "Displaying top " << n << " log lines...\n";
        std::cout << tail(SERVER_LOG, n);
    } catch (std::exception& ex) {
        std::cout << "Could not read number of log lines!\n";
    }
}

void handleTransmissionAcceleration() {
    if (serverContext.delay > serverContext.min_delay &&
        getTimeInMicroseconds() - serverContext.last_acceleration >
            serverContext.transmission_acceleration_interval) {
        if (serverContext.delay < serverContext.min_delay + serverContext.acceleration_rate)
            serverContext.delay = serverContext.min_delay;
        else
            serverContext.delay -= serverContext.acceleration_rate;
        serverContext.last_acceleration = getTimeInMicroseconds();
        DLOG_S(INFO) << fmt::format("Auto transmission increase. New delay {}",
                                    serverContext.delay);
    }
}

void handleTransmissionSlowdown() {
    if (serverContext.delay < serverContext.max_delay &&
        getTimeInMicroseconds() - serverContext.last_slowdown > serverContext.slowdown_interval) {
        if (serverContext.delay + serverContext.slowdown_rate > serverContext.max_delay)
            serverContext.delay = serverContext.max_delay;
        else
            serverContext.delay += serverContext.slowdown_rate;
        serverContext.last_slowdown = getTimeInMicroseconds();
        serverContext.last_acceleration = serverContext.last_slowdown;
        DLOG_S(INFO) << fmt::format("Received CONTROL SLOW DOWN TRANSMISSION. New delay {}",
                                    serverContext.delay);
    }
}

//
// THREADS
//

void* receive(void*) {
    sockaddr_in6 from;
    socklen_t from_len;

    while (!should_close.load()) {
        std::string bytes = conn.recvMsgFrom(&from, &from_len);
        DLOG_S(INFO) << bytes.length() << " bytes recved from: " << addrToStr(&from.sin6_addr);
        msgq.push(bytes, from, from_len);
    }

    return nullptr;
}

void* compute(void*) {
    std::unique_ptr<Message> msg;
    while (!should_close.load()) {
        MsgQueueEl qel = msgq.pop();
        std::string bytes = qel.payload;
        auto c_it = bytes.cbegin();
        try {
            msg = deserialize(c_it, bytes.cend());
        } catch (std::exception& e) {
            LOG_S(ERROR) << fmt::format("Error deserializing message: {}", e.what());
            continue;
        }

        handleTransmissionAcceleration();

        if (msg->GetType() == TYPE_CONTROL_MESSAGE) {
            auto ctrl_msg = static_cast<ControlMessage*>(msg.get());
            ControlMsgType msg_type = ctrl_msg->control_msg_type;

            if (msg_type == CONTROL_NAK || msg_type == CONTROL_FILE_HEADER_REQUESTED) {
                std::lock_guard fss_lock(fss_m);
                std::lock_guard pi_lock(pi_m);
                handleNakAndFileHeaderRequested(ctrl_msg, conn, fss, pi);
            } else if (msg_type == CONTROL_SLOW_DOWN_TRANSMISSION) {
                handleTransmissionSlowdown();
            }
        } else if (msg->GetType() == TYPE_PANEL_HELLO) {
            std::lock_guard pi_lock(pi_m);
            handlePanelHello(std::move(msg), conn, pi, qel.from);
        }
    }

    return nullptr;
}

void* interact(void*) {
    CLI cli{};
    cli.registerCommand("schedule", "<file> - sends schedule file", handleSchedule);
    cli.registerCommand("panic",
                        "<file> (--name <panel_name>)|(--tags <tags>) - sends important conent",
                        handlePanic);
    cli.registerCommand("top", "<n> - display top n log lines", handleTop);
    cli.registerCloseCommand("bye");

    cli.start();

    should_close.store(true);

    return nullptr;
}

//
// MAIN
//

int main(int argc, char** argv) {
    loggerInit(argc, argv, SERVER_LOG, !LOG_TO_CONSOLE);

    LOG_S(INFO) << "UDP server running on port: " << conn.getPort();
    LOG_S(ERROR) << "For server only error is showed on console\n";

    conn.connect(strToSockaddr(MC_IP, MC_PORT));

    pthread_t receiver, computer, interacter;

    pthread_create(&receiver, NULL, receive, NULL);
    pthread_create(&computer, NULL, compute, NULL);
    pthread_create(&interacter, NULL, interact, NULL);

    pthread_join(receiver, NULL);
    pthread_join(computer, NULL);
    pthread_join(interacter, NULL);

    return 0;
}
