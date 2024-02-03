/*
 * Project: TinyAd
 * Description: Command Line Interface utility class
 * Author: Kamil Przybyła
 * Created: 26.05.2021
 */

#ifndef CLI_HPP
#define CLI_HPP

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace TinyAd {

class CLI {
public:
    struct Command {
        std::string name;
        std::optional<std::string> value;
        std::unordered_map<std::string, std::string> args;
    };

    CLI() : commands_(), help_messages_(), close_command_("exit") {}

    void start();
    void registerCommand(const std::string& name, std::function<void(const Command&)> handler);
    void registerCommand(const std::string& name, const std::string& help,
                         std::function<void(const Command&)> handler);
    void registerCloseCommand(const std::string& name);

private:
    void printHelp();
    std::vector<std::string> splitLine(const std::string& line);
    Command createCommand(const std::string& line);

    std::unordered_map<std::string, std::function<void(const Command&)>> commands_;
    std::unordered_map<std::string, std::string> help_messages_;
    std::string close_command_;
};

}  // namespace TinyAd

#endif