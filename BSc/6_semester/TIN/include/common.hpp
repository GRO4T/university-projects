/*
 * Project: TinyAd
 * Description: Common utilities
 * Author: Damian Kolaska
 * Created: 30.04.2021
 */

#ifndef COMMON_HPP
#define COMMON_HPP

#include <fmt/core.h>

#include <exception>
#include <loguru.hpp>
#include <map>
#include <optional>
#include <random>

#include "CRC.hpp"
#include "uuid.h"

namespace TinyAd {

class UniformRealDistribution {
public:
    UniformRealDistribution(double min, double max) {
        std::random_device rd;
        gen_ = std::mt19937(rd());
        dis_ = std::uniform_real_distribution<>(min, max);
    };

    double get() { return dis_(gen_); };

private:
    std::mt19937 gen_;
    std::uniform_real_distribution<> dis_;
};

class UniformIntDistribution {
public:
    UniformIntDistribution(int min, int max) {
        std::random_device rd;
        gen_ = std::mt19937(rd());
        dis_ = std::uniform_int_distribution<>(min, max);
    };

    int get() { return dis_(gen_); };

private:
    std::mt19937 gen_;
    std::uniform_int_distribution<> dis_;
};

class UuidMaker {
public:
    /**
     * Provides reference to the singleton instance of UuidMaker.
     */
    static UuidMaker& Instance() {
        static UuidMaker uuid_maker;
        return uuid_maker;
    }

    /**
     * Generates random UUID using default settings.
     */
    uuids::uuid GenUuid() { return (*gen)(); }

private:
    UuidMaker() {
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size>{};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        static std::mt19937 generator(seq);
        gen =
            std::make_unique<uuids::uuid_random_generator>(uuids::uuid_random_generator{generator});
    }
    ~UuidMaker() = default;
    UuidMaker(const UuidMaker&) = delete;
    UuidMaker& operator=(const UuidMaker&) = delete;

    std::unique_ptr<uuids::uuid_random_generator> gen;
};

const bool LOG_TO_CONSOLE = true;

inline void loggerInit(int argc, char** argv, const std::string& logfile = "tinyad.log",
                       bool log_to_console = true,
                       loguru::Verbosity verbosity = loguru::Verbosity_MAX) {
    loguru::init(argc, argv);
    loguru::add_file(logfile.c_str(), loguru::Append, verbosity);
    // Only show most relevant things on stderr:
    if (log_to_console)
        loguru::g_stderr_verbosity = loguru::Verbosity_0;
    else
        loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
}

template <class K, class V>
std::optional<V> getValueFromMap(const std::map<K, V>& map, const K& key) {
    try {
        return map.at(key);
    } catch (std::out_of_range& e) {
        return std::nullopt;
    }
}

inline uint32_t calculateCRC(const std::string& data) {
    return CRC::Calculate(data.c_str(), data.length(), CRC::CRC_32());
}

}  // namespace TinyAd

#endif