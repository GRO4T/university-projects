#include "inc/algorithm/graph.hpp"

std::unique_ptr<CMYKGraph::Node> CMYKGraph::set_start(const std::string& cmyk) {
    return std::make_unique<CMYKGraph::Node>(last_id++, 0, cmyk);
}

std::vector<std::unique_ptr<CMYKGraph::Node>> CMYKGraph::get_neighbours(const std::unique_ptr<CMYKGraph::Node>& node) {
    std::vector<std::unique_ptr<CMYKGraph::Node>> neighbours;

    for (std::size_t i=0; i < node->state().length()-3; ++i) {
        Robot robot(node->state());
        robot.move(i);
        neighbours.push_back(std::make_unique<CMYKGraph::Node>(last_id++, i, std::move(robot)));
    }

    return neighbours;
}

void CMYKGraph::set_predecessor(const std::unique_ptr<CMYKGraph::Node>& predecessor, const std::unique_ptr<CMYKGraph::Node>& current) {
    came_from[current->id] = predecessor->id;
    made_move[current->id] = current->move;
}

std::vector<std::size_t> CMYKGraph::reconstruct_path(const std::unique_ptr<CMYKGraph::Node>& from) {
    std::vector<std::size_t> path;
    std::size_t id = from->id;

    while (came_from.contains(id)) {
        auto next_id = came_from.at(id);
        path.push_back(made_move.at(id));
        id = next_id;
    }

    std::reverse(std::begin(path), std::end(path));

    return path;
}

bool CMYKGraph::Node::valid() const noexcept {
    const auto & array = robot.state();
    for (std::size_t i=0; i < (array.length() & ~0x03ULL ); ++i)
        if (array[i] != "CMYK"[i % 4])
            return false;
    return true;
}


