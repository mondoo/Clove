#pragma once

#include "Clove/Reflection/Common.hpp"

#include <fstream>
#include <string>
#include <vector>

namespace garlic::clove {
    struct Node {
        std::string name;
        std::vector<Node> children{};
    };

    class Serialiser {
        //VARIABLES
    protected://TODO: Private with protected getters
        Node root{};
        Node *head{ nullptr };

        //FUNCTIONS
    public:
        virtual ~Serialiser() = default;

        //TODO: inl

        void push(std::string_view name) {
            if(head == nullptr){
                head = &root;
                head->name = name;
            }else{
                Node node{};
                node.name = name;
                head->children.emplace_back(node);
            }
        }

        /* template<typename T>
        void push(T &&type, std::string_view name) {
            
        } */

        //TODO: ofsteam override. Takes a file name
        virtual std::string emitt() = 0;

    private:
        //virtual void addClass(Class class) = 0;
    };
}