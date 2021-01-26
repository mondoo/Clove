#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace garlic::clove {
    class Serialiser {
        //TYPES
    public:
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

        //VARIABLES
    protected://TODO: Private with protected getters
        Node root{};
        Node *head{ nullptr };

        //FUNCTIONS
    public:
        virtual ~Serialiser() = default;

        //TODO: Have begin and end for parent nodes and set for leaf nodes.

        /**
         * @brief Push an empy node.
         * @param name Name of the node.
         */
        void push(std::string_view name);

        /**
         * @brief Push a node with a value.
         * @param name Name of the node.
         * @param value Value of the node.
         */
        void push(std::string_view name, float value);

        /**
         * @brief Push back a generic type. Expects a specialisation of serialise for the type.
         * @param name Overrides the name of the root node for the type.
         * @param object The object to serialise.
         */
        template<typename T>
        void push(std::string_view name, T const &object);

        /**
         * @brief Emitts the current node heriachy as a string.
         */
        virtual std::string emitt() = 0;
    };
}

namespace garlic::clove {
    template<typename UnkownType>
    Serialiser::Node serialise(UnkownType const &object) {
        return {};
    }

    template<typename UnkownType>
    UnkownType deserialise(Serialiser::Node const &node) {
        return {};
    }
}

#include "Serialiser.inl"