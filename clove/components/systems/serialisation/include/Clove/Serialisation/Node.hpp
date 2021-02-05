#pragma once

#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

namespace garlic::clove::serialiser {
    struct Node;
}

namespace garlic::clove::serialiser {
    struct Node {
        std::string name;
        std::variant<float, std::string, std::vector<Node>> value{};

        Node();
        Node(std::string name);
        Node(std::string_view name);

        Node(Node const &other);
        Node(Node &&other) noexcept;

        Node &operator=(Node const &other);
        Node &operator=(Node &&other) noexcept;

        template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        Node &operator=(T scalar);

        Node &operator=(std::string string);
        Node &operator=(char const *string);

        template<typename T, std::enable_if_t<!std::is_arithmetic_v<T>, int> = 0>
        Node &operator=(const T &object);

        ~Node();

        template<typename T>
        T as() const;

        Node &operator[](std::string_view nodeName);
        Node const &operator[](std::string_view nodeName) const;
    };
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

#include "Node.inl"