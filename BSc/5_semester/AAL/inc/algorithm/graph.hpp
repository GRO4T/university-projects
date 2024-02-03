#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "robot.hpp"

/*
 * Graph implementation used for our search-based algorithms
 */
class CMYKGraph {
    std::size_t last_id = 0;
    std::unordered_map<std::size_t, std::size_t> came_from;
    std::unordered_map<std::size_t, std::size_t> made_move;

public:
    struct Node {
        std::size_t id;
        std::size_t move;
        Robot robot;

    public:
        Node(std::size_t id, std::size_t move, Robot robot)
            : id(id), move(move), robot(std::move(robot)) {}
        const std::string& state() const noexcept { return robot.state(); }
        bool valid() const noexcept;
    };

    std::unique_ptr<Node> set_start(const std::string& cmyk);
    std::vector<std::unique_ptr<Node>> get_neighbours(const std::unique_ptr<Node>& node);
    void set_predecessor(const std::unique_ptr<Node>& predecessor,
                         const std::unique_ptr<Node>& current);
    std::vector<std::size_t> reconstruct_path(const std::unique_ptr<Node>& from);
};

#endif
