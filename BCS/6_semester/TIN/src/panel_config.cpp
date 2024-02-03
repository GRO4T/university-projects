/*
 * Project: TinyAd
 * Description: Panel configuration that utilizes
 *              generic key-value config parser
 * Author: Kamil Przybyła
 * Created: 19.05.2021
 */

#include "panel_config.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "config_parser.hpp"

namespace TinyAd {

PanelConfig::PanelConfig(const std::string& source) : source_(source) {
    std::stringstream source_stream{source};
    ConfigParser parser{source_stream};
    const Config config = parser.parseConfig();

    if (config.blocks.size() != 1)
        throw PanelConfigParserException("Unexpected number of blocks in panel config source!");

    parseConfig(config.blocks[0]);
}

PanelConfig PanelConfig::fromFile(const std::string filename) {
    std::ifstream file(filename);

    if (!file.is_open()) throw PanelConfigParserException("File " + filename + " not found!");

    std::stringstream source_stream;
    source_stream << file.rdbuf();
    file.close();

    return PanelConfig(source_stream.str());
}

bool PanelConfig::hasTag(const std::string& tag) const {
    const auto it = std::find(tags_.begin(), tags_.end(), tag);
    return it != tags_.end();
}

std::string PanelConfig::toString() const {
    std::string str = "\n";

    str += "Panel name: " + name_ + "\n";
    str += "Server IP: " + server_ip_ + "\n";
    str += "Tags:\n";
    for (const auto& tag : tags_) {
        str += "\t" + tag + "\n";
    }
    str += "Num retires: " + std::to_string(num_retries_) + "\n";
    str += "Timeout: " + std::to_string(timeout_) + "\n";

    return str;
}

void PanelConfig::parseConfig(const ConfigBlock& config_block) {
    server_ip_ = config_block.getValue("server_ip").getString();
    name_ = config_block.getValue("panel_name").getString();
    num_retries_ = std::stoul(config_block.getValue("num_retries").getString());
    timeout_ = std::stoul(config_block.getValue("timeout").getString());

    const auto tags = config_block.tryGetValue("tags");
    if (tags) {
        tags_ = tags.value().parseList();
    }
}

}  // namespace TinyAd