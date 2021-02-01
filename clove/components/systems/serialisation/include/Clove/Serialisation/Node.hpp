#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <type_traits>

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

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        Node &operator=(T scalar) {
            value = static_cast<float>(scalar);
            return *this;
        }

        template<typename T, std::enable_if_t<!std::is_arithmetic_v<T>, int> = 0>
        Node &operator=(const T &object) {
            std::string const prevName{ name };
            *this = serialise(object);
            name  = prevName;

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

            nodes.emplace_back(Node{
                .name = std::string{ nodeName },
            });

            return nodes.back();
        }
    };
}