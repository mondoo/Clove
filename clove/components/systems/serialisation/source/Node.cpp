#include "Clove/Serialisation/Node.hpp"

namespace garlic::clove::serialiser {
    Node::Node() = default;

    Node::Node(Node const &other) = default;

    Node::Node(Node &&other) noexcept = default;

    Node &Node::operator=(Node const &other) {
        if(other.type == Type::None){
            scalar = other.scalar;
        }
        nodes = other.nodes;
        type  = other.type;

        return *this;
    }

    Node &Node::operator=(Node &&other) noexcept {
        if(other.type == Type::None) {
            scalar = std::move(other.scalar);
        }
        nodes = std::move(other.nodes);
        type  = std::move(other.type);

        return *this;
    }

    Node &Node::operator=(char const *string) {
        return operator=<char const *>(string);
    }

    Node::~Node() = default;

    Node &Node::operator[](std::string_view nodeName) {
        //TODO: Seq -> Map conversion
        type = Type::Map;

        for(auto &node : nodes) {
            if(node.scalar == nodeName) {
                return node;
            }
        }

        return nodes.emplace_back(Node{ nodeName });
    }

    Node const &Node::operator[](std::string_view nodeName) const {
        if(type == Type::None) {
            throw std::runtime_error{ "Node does not contain child nodes." };
        }

        for(auto const &node : nodes) {
            if(node.scalar == nodeName) {
                return node;
            }
        }

        throw std::runtime_error{ "Node does not have requested child." };
    }

    Node::Node(std::string_view key)
        : scalar{ key } {
        Node child{};
        child.type = Type::Scalar;
        nodes.emplace_back(std::move(child));
    }
}