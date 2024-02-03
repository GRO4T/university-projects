/*
 * Project: TinyAd
 * Description: Generic key-value config parser
 * Author: Kamil Przybyła
 * Created: 19.05.2021
 */

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "exceptions.hpp"

namespace TinyAd {

class ConfigParserException : public TinyAdException {
public:
    ConfigParserException(const std::string& msg) { msg_ = msg; }
};

class ConfigValue {
public:
    ConfigValue(const std::string& value) : value_(value) {}

    const std::string& getString() const { return value_; }

    std::vector<std::string> parseList() const;

private:
    std::string value_;
};

class ConfigBlock {
public:
    ConfigBlock() : key_values_() {}
    ConfigBlock(const std::unordered_map<std::string, ConfigValue>& key_values)
        : key_values_(key_values) {}

    void addKeyValue(const std::string& key, const std::string& value);

    const ConfigValue& getValue(const std::string& key) const;
    std::optional<ConfigValue> tryGetValue(const std::string& key) const;

private:
    std::unordered_map<std::string, ConfigValue> key_values_;
};

struct Config {
    void addBlock(const ConfigBlock& block) { blocks.emplace_back(block); }

    std::vector<ConfigBlock> blocks;
};

class ConfigParser {
public:
    ConfigParser(std::istream& stream) : stream_(stream) {}

    Config parseConfig();

private:
    std::istream& stream_;
};

}  // namespace TinyAd

#endif