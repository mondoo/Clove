#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace garlic::clove::serialiser {
    struct Node;
}

namespace garlic::clove {
    template<typename UnkownType>
    serialiser::Node serialise(UnkownType const &object) {
        return {};
    }

    template<typename UnkownType>
    UnkownType deserialise(serialiser::Node const &node) {
        return {};
    }
}

namespace garlic::clove::serialiser {
    struct Node {
        std::string name;
        std::variant<float, std::vector<Node>> value{};

        Node &operator=(float scalar) {
            value = scalar;
            return *this;
        }

        Node &operator[](std::string_view nodeName) {
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

            Node node{};
            node.name = nodeName;
            nodes.push_back(std::move(node));

            return nodes.back();
        }
    };
}