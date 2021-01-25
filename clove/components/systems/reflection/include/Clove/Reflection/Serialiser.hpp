#pragma once

#include "Clove/Reflection/Common.hpp"

#include <fstream>
#include <string>
#include <variant>
#include <vector>

namespace garlic::clove {
    class Serialiser {
        //TYPES
    public:
        struct Node {
            std::string name;
            std::variant<float, std::vector<Node>> value{};
        };

        //VARIABLES
    protected://TODO: Private with protected getters
        Node root{};
        Node *head{ nullptr };

        //FUNCTIONS
    public:
        virtual ~Serialiser() = default;

        //TODO: inl

        //TODO: Have begin and end for parent nodes and set for leaf nodes.

        void push(std::string_view name) {
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

        //template<typename T>
        void push(std::string_view name, float value) {
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

        template<typename T>
        void push(std::string_view name, T const &type) {
            Type typeInfo{ reflect(type) };

            push(typeInfo.name);
            for(auto const &field : typeInfo.fields){
                //TEMP: All members are floats
                push(field.name, std::any_cast<float>(field.value));
            }
        }

        //TODO: ofsteam override. Takes a file name
        virtual std::string emitt() = 0;

    private:
        //virtual void addClass(Class class) = 0;
    };
}