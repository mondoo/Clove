#include "Clove/Serialisation/YamlSerialiser.hpp"

#include <sstream>
#include <yaml-cpp/yaml.h>

namespace garlic::clove {
    namespace {
        void emittNode(YAML::Node &emitterNode, Serialiser::Node const &node) {
            if(node.name.length() <= 0) {
                return;
            }

            if(auto *floatVal{ std::get_if<float>(&node.value) }) {
                emitterNode[node.name] = *floatVal;
            } else if(auto *children{ std::get_if<std::vector<Serialiser::Node>>(&node.value) }; children != nullptr && children->size() > 0) {
                YAML::Node childNode{};
                for(auto &child : *children) {
                    emittNode(childNode, child);
                }
                emitterNode[node.name] = childNode;
            }
        }

        namespace v1 {
            std::string emitt(Serialiser::Node const &rootNode) {
                YAML::Node emitterNode{};

                emitterNode["type"]    = "yaml";
                emitterNode["version"] = 1;

                emittNode(emitterNode, rootNode);

                YAML::Emitter emitter{};
                emitter << emitterNode;

                std::stringstream stream;
                stream << emitter.c_str();

                return stream.str();
            }
        }
    }

    std::string YamlSerialiser::emitt() {
        return v1::emitt(root);
    }
}