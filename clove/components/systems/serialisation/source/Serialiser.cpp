#include "Clove/Serialisation/Serialiser.hpp"

namespace garlic::clove {
    void Serialiser::push(std::string_view name) {
        push(Node{
            .name = std::string{ name },
        });
    }

    void Serialiser::push(Node node) {
        if(auto *children{ std::get_if<std::vector<Node>>(&root.value) }) {
            children->emplace_back(std::move(node));
        } else {
            root.value = std::vector<Node>{ std::move(node) };
        }
    }

    void Serialiser::push(std::string_view name, float value) {
        push(Node{
            .name  = std::string{ name },
            .value = value,
        });
    }
}