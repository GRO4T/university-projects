/*
 * Project: TinyAd
 * Description: Generic key-value config parser
 * Author: Kamil Przybyła
 * Created: 19.05.2021
 */

#include "config_parser.hpp"

namespace TinyAd {

std::vector<std::string> ConfigValue::parseList() const {
    std::vector<std::string> items{};

    bool inside_parenthesis = false;
    size_t start_idx = 0;
    for (size_t idx = 0; idx < value_.size(); ++idx) {
        const char c = value_[idx];

        if (!inside_parenthesis && c == '[') {
            inside_parenthesis = true;
            start_idx = idx + 1;
        } else if (inside_parenthesis && c == '[')
            throw ConfigParserException("Expected list item!");
        else if (!inside_parenthesis && c != '[')
            throw ConfigParserException("Expected opening parenthesis!");
        else if (inside_parenthesis && c == ']') {
            const auto item = value_.substr(start_idx, idx - start_idx);
            items.emplace_back(item);
            inside_parenthesis = false;
        }
    }

    if (inside_parenthesis) throw ConfigParserException("Expected closing parenthesis!");

    return items;
}

void ConfigBlock::addKeyValue(const std::string& key, const std::string& value) {
    key_values_.emplace(key, ConfigValue{value});
}

const ConfigValue& ConfigBlock::getValue(const std::string& key) const {
    const auto it = key_values_.find(key);
    if (it == key_values_.end())
        throw ConfigParserException("Property '" + key +
                                    "' was requested, but could not be found!");

    return it->second;
}

std::optional<ConfigValue> ConfigBlock::tryGetValue(const std::string& key) const {
    const auto it = key_values_.find(key);
    if (it == key_values_.end())
        return {};
    else
        return it->second;
}

Config ConfigParser::parseConfig() {
    Config conf{};

    ConfigBlock block{};

    std::string line;
    while (std::getline(stream_, line)) {
        if (line.size() == 0) continue;
        if (line == "---") {
            conf.addBlock(block);
            block = ConfigBlock{};
        } else {
            const auto idx = line.find('=');
            if (idx == std::string::npos) throw ConfigParserException("Expected key-value pair!");

            const auto key = line.substr(0, idx);
            const auto value = line.substr(idx + 1);

            block.addKeyValue(key, value);
        }
    }
    conf.addBlock(block);

    return conf;
}

}  // namespace TinyAd