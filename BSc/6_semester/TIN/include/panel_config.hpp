/*
 * Project: TinyAd
 * Description: Panel configuration that utilizes
 *              generic key-value config parser
 * Author: Kamil Przybyła
 * Created: 19.05.2021
 */

#ifndef PANEL_CONFIG_HPP
#define PANEL_CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>

#include "config_parser.hpp"
#include "exceptions.hpp"

namespace TinyAd {

class PanelConfigParserException : public TinyAdException {
public:
    PanelConfigParserException(const std::string& msg) { msg_ = msg; }
};

class PanelConfig {
public:
    PanelConfig(const std::string& source);

    static PanelConfig fromFile(const std::string filename);

    void addTag(const std::string& tag) { tags_.emplace_back(tag); }

    bool hasTag(const std::string& tag) const;

    const std::string getStringSource() const { return source_; }
    const std::string& getName() const { return name_; }
    const std::string& getServerIp() const { return server_ip_; }
    const std::vector<std::string>& getTags() const { return tags_; }
    size_t getNumRetries() const { return num_retries_; }
    size_t getTimeout() const { return timeout_; }

    std::string toString() const;

private:
    void parseConfig(const ConfigBlock& config_block);

    std::string source_;
    std::string name_;
    std::string server_ip_;
    std::vector<std::string> tags_;
    size_t num_retries_;
    size_t timeout_;
};

}  // namespace TinyAd

#endif