#include "Clove/Serialisation/Serialiser.hpp"

namespace garlic::clove {
    void Serialiser::push(std::string_view name) {
        if(head == nullptr) {
            head       = &root;
            head->name = name;
        } else {
            Node node{};
            node.name = name;
            if(auto *children{ std::get_if<std::vector<Node>>(&head->value) }) {
                children->emplace_back(std::move(node));
            } else {
                head->value = std::vector<Node>{ std::move(node) };
            }
        }
    }

    void Serialiser::push(std::string_view name, float value) {
        if(head == nullptr) {
            head        = &root;
            head->name  = name;
            head->value = value;
        } else {
            Node node{};
            node.name  = name;
            node.value = value;
            if(auto *children{ std::get_if<std::vector<Node>>(&head->value) }) {
                children->emplace_back(std::move(node));
            } else {
                head->value = std::vector<Node>{ std::move(node) };
            }
        }
    }
}