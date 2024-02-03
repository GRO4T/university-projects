/*
 * Project: TinyAd
 * Description: Advertising panel (client) implementation
 * Authors: Damian Kolaska
 *          Rafał Kulus
 * Created: 03.05.2021
 */

#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include <ctime>
#include <filesystem>
#include <mutex>

#include "client/utils.hpp"
#include "common.hpp"
#include "config.hpp"
#include "message_deserializer.hpp"
#include "message_queue.hpp"
#include "panel_config.hpp"
#include "time.hpp"

using namespace TinyAd;

//
// GLOBALS
//

std::string panel_id;

std::optional<PanelConfig> conf = {};
std::optional<Schedule> schedule = {};

// Otherwise known as 'Have I got ServerConnectionAccept at least once?'
bool joined_mc_group = false;

WeakUdpConnection conn(0.1, MC_PORT, true);

// MessageQueue has built-in sync, no need for mutex
MessageQueue msgq(4000);

std::mutex fds_m;
FileDeserializerMap fds;

std::mutex mh_m;
MissingHeadersSet mh;

//
// THREADS
//

void* receive(void*) {
    while (true) {
        std::string bytes = conn.recvMsg();
        DLOG_S(INFO) << bytes.length() << " bytes recved";
        if (!msgq.push(bytes)) {
            DLOG_S(INFO) << "Message Queue overflown";
            auto ctl_msg = ControlMessage(panel_id, CONTROL_SLOW_DOWN_TRANSMISSION);
            conn.sendMsg(serialize(ctl_msg));
        }
    }
}

void* compute(void*) {
    std::string new_sched_id;
    std::unique_ptr<Message> msg;
    while (true) {
        std::string bytes = msgq.pop().payload;
        auto c_it = bytes.cbegin();
        try {
            msg = deserialize(c_it, bytes.cend());
        } catch (std::exception& e) {
            LOG_S(ERROR) << fmt::format("Error deserializing message: {}", e.what());
            continue;
        }

        std::lock_guard fds_lock(fds_m);
        std::lock_guard mh_lock(mh_m);

        switch (msg->GetType()) {
            case TYPE_MULTIMEDIA_HEADER:
                handleMultimediaHeader(std::move(msg), fds, mh, new_sched_id);
                break;
            case TYPE_MULTIMEDIA_DATA:
                handleMultimediaData(std::move(msg), fds, mh, new_sched_id, schedule);
                break;
            case TYPE_CONTROL_MESSAGE: {
                auto ctrl_msg = static_cast<ControlMessage*>(msg.get());

                if (ctrl_msg->control_msg_type == CONTROL_FILE_NOT_FOUND) {
                    handleControlFileNotFound(ctrl_msg, fds, mh);
                }
            } break;
            case TYPE_SERVER_CONN_ACCEPT:
                handleServerConnAccept(std::move(msg), conn, joined_mc_group);
                break;
        }
    }
}

void* tick(void*) {
    time_t next_long_tick = 0;

    UniformIntDistribution short_tick_generator(5, 20);

    while (true) {
        usleep(short_tick_generator.get());

        std::lock_guard fds_lock(fds_m);
        std::lock_guard mh_lock(mh_m);

        sendRandomNaks(conn, fds, panel_id);
        sendSingleFileHeaderRequested(conn, mh, panel_id);

        if (next_long_tick < std::time(0)) {
            // PanelHello also acts as a 'keep alive'
            DLOG_S(INFO) << "Sending PanelHello...";
            PanelHello hello(panel_id, conf.value().getStringSource());
            conn.sendMsg(serialize(hello));

            printActiveFiles(fds);
            printCurrentContent(schedule);

            next_long_tick = std::time(0) + conf.value().getTimeout();
        }
    }
}

//
// MAIN
//

int main(int argc, char** argv) {
    loggerInit(argc, argv, "logs/client.log");

    panel_id = uuids::to_string(UuidMaker::Instance().GenUuid());
    LOG_S(INFO) << fmt::format("Panel: {} started\n", panel_id);

    LOG_S(INFO) << "UDP client running on port: " << conn.getPort();

    try {
        if (argc == 2) {
            conf = PanelConfig::fromFile(argv[1]);
        } else {
            conf = PanelConfig::fromFile("panel.conf");
        }
    } catch (std::exception& e) {
        LOG_S(ERROR) << "Error while loading config file: " << e.what();
        return -1;
    }

    std::filesystem::create_directory("recvd");

    conn.connect(strToSockaddr(conf.value().getServerIp(), SRV_PORT));

    pthread_t receiver, computer, ticker;

    pthread_create(&receiver, NULL, receive, NULL);
    pthread_create(&computer, NULL, compute, NULL);
    pthread_create(&ticker, NULL, tick, NULL);

    pthread_join(receiver, NULL);
    pthread_join(computer, NULL);
    pthread_join(ticker, NULL);

    return 0;
}
