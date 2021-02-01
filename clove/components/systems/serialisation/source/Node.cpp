#include "Clove/Serialisation/Node.hpp"

namespace garlic::clove::serialiser {
    Node::Node() = default;
    Node::Node(std::string name)
        : name{ std::move(name) } {
    }
    Node::Node(std::string_view name)
        : name{ name } {
    }

    Node::Node(Node const &other)     = default;
    Node::Node(Node &&other) noexcept = default;

    Node &Node::operator=(Node const &other) {
        //Note: Not taking name on assignment only
        value = other.value;
        return *this;
    }

    Node &Node::operator=(Node &&other) noexcept {
        value = std::move(other.value);
        return *this;
    }

    Node::~Node() = default;

    Node &Node::operator[](std::string_view nodeName) {
        //Turn this node into a parent node if not already
        if(!std::holds_alternative<std::vector<Node>>(value)) {
            value = std::vector<Node>{};
        }

        auto &nodes{ std::get<std::vector<Node>>(value) };

        for(auto &node : nodes) {
            if(node.name == nodeName) {
                return node;
            }
        }

        nodes.emplace_back(nodeName);

        return nodes.back();
    }
}