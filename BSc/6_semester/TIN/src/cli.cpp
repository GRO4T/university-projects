/*
 * Project: TinyAd
 * Description: Command Line Interface utility class
 * Author: Kamil Przybyła
 * Created: 26.05.2021
 */

#include "cli.hpp"

#include <iostream>

#include "common.hpp"

namespace TinyAd {

std::vector<std::string> CLI::splitLine(const std::string& line) {
    size_t i = 0;
    while (line[i] == ' ') i += 1;

    std::vector<std::string> splitted{};

    size_t element_start = i;
    bool in_quotes = false;
    bool in_free_element = false;

    while (i < line.size()) {
        if (line[i] == '\"') {
            if (!in_quotes) {
                element_start = i + 1;
            } else {
                if (i + 1 < line.size() && line[i + 1] != ' ')
                    throw std::runtime_error("Unexpected character!");

                const auto element = line.substr(element_start, i - element_start);
                splitted.emplace_back(element);
                i += 1;
                element_start = i + 1;
            }
            in_quotes = !in_quotes;
            in_free_element = false;
        } else if (!in_quotes && line[i] == ' ') {
            const auto element = line.substr(element_start, i - element_start);
            if (!element.empty()) splitted.emplace_back(element);
            element_start = i + 1;
            in_free_element = false;
        } else {
            in_free_element = true;
        }

        i += 1;
    }

    if (in_quotes) throw std::runtime_error("Unexpected end of command!");

    if (in_free_element) {
        const auto element = line.substr(element_start, i - element_start);
        splitted.emplace_back(element);
    }

    return splitted;
}

CLI::Command CLI::createCommand(const std::string& line) {
    const auto splitted = splitLine(line);
    std::unordered_map<std::string, std::string> args{};

    Command command{};
    command.name = splitted.size() > 0 ? splitted[0] : "";

    std::optional<std::string> arg_name{};
    for (size_t i = 1; i < splitted.size(); ++i) {
        const auto& element = splitted[i];

        if (element[0] == '-' && element[1] == '-') {
            arg_name = element.substr(2);
        } else if (arg_name) {
            command.args.insert({arg_name.value(), element});
        } else if (!command.value) {
            command.value = element;
        } else {
            throw std::runtime_error("Command should have only one value!");
        }
    }

    return command;
}

void CLI::printHelp() {
    std::cout << "Usage:\n";
    for (const auto& command : commands_) {
        std::cout << "\t" << command.first << " " << help_messages_[command.first] << "\n";
    }
    std::cout << "\t" << close_command_ << " - exit\n";
}

void CLI::start() {
    bool should_close = false;
    std::string line;
    while (!should_close) {
        std::cout << ">";
        std::getline(std::cin, line);

        if (std::cin.eof()) break;

        try {
            const auto command = createCommand(line);
            if (command.name == close_command_) {
                should_close = true;
            } else if (command.name == "help") {
                printHelp();
            } else {
                const auto it = commands_.find(command.name);
                if (it != commands_.end()) {
                    it->second(command);
                } else {
                    std::cout << "Unrecognized command! Type 'help' to get help.\n";
                }
            }
        } catch (std::exception& e) {
            LOG_S(ERROR) << fmt::format("CLI error: {}", e.what());
        }
    }
}

void CLI::registerCommand(const std::string& name, std::function<void(const Command&)> handler) {
    registerCommand(name, "", handler);
}

void CLI::registerCommand(const std::string& name, const std::string& help,
                          std::function<void(const Command&)> handler) {
    commands_.insert({name, handler});
    help_messages_.insert({name, help});
}

void CLI::registerCloseCommand(const std::string& name) { close_command_ = name; }

}  // namespace TinyAd