#ifndef __PROPERTY_HPP__
#define __PROPERTY_HPP__

#include <chrono>
#include <utility>

#include "algorithm/alg_params.hpp"

/*
 * Builds AlgorithmResult containing algorithm's execution info
 */
class PropertyBuilder {
public:
    inline PropertyBuilder& set_moves(const std::vector<std::size_t>& moves) {
        result.moves = moves;
        return *this;
    }
    inline PropertyBuilder& set_sorted_string(const std::string& str) {
        return set_property("Result String", str);
    }
    inline PropertyBuilder& set_success(bool success) {
        return set_property("Result State", success ? "Success" : "Fail");
    }
    inline PropertyBuilder& set_fail_reson(const std::string& reson) {
        return set_property("Fail Reson", reson);
    }
    inline PropertyBuilder& set_execution_time(const std::chrono::seconds& seconds) {
        return set_property("Execution Time", std::to_string(seconds.count()) + " s");
    }

    inline PropertyBuilder& set_property(const std::string& name, const std::string& value) {
        result.results.insert(std::make_pair(name, value));
        return *this;
    }
    inline const AlgorithmResult& build() const { return result; }

    operator AlgorithmResult() { return build(); }

private:
    AlgorithmResult result;
};

/*
 * Class containing informatino about execution of the algorithm
 */
class PropertyParser {
public:
    PropertyParser(const AlgorithmResult& result) : result(result) {}

    inline const std::vector<std::size_t>& get_moves() { return result.moves; }
    inline const std::string& get_sorted_string() { return get_property("Result String"); }
    inline const std::string& get_success() { return get_property("Result State"); }
    inline const std::string& get_fail_reson() { return get_property("Fail Reson"); }
    inline const std::string& get_execution_time() { return get_property("Execution Time"); }

    inline const std::string& get_property(const std::string& name) {
        return result.results.at(name);
    }

private:
    AlgorithmResult result;
};

#endif
