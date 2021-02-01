#pragma once

#include <string>
#include <string_view>
#include <type_traits>
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

        Node() = default;
        Node(std::string name)
            : name{ std::move(name) } {
        }
        Node(std::string_view name)
            : name{ name } {
        }

        Node(Node const &other) = default;
        Node(Node &&other)      = default;

        Node &operator=(Node const &other) {
            //Note: Not taking name on assignment only
            value = other.value;
            return *this;
        }

        Node &operator=(Node &&other) noexcept {
            value = std::move(other.value);
            return *this;
        }

        ~Node() = default;

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

            nodes.emplace_back(nodeName);

            return nodes.back();
        }
    };
}