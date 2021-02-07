#include "Clove/Serialisation/Yaml.hpp"

#include <sstream>
#include <yaml-cpp/yaml.h>

using namespace garlic::clove::serialiser;

namespace garlic::clove {
    namespace {
        void emittNode(YAML::Node &emitterNode, Node const &node) {
            if(node.name.length() <= 0) {
                return;
            }

            if(auto const *floatVal{ std::get_if<float>(&node.value) }) {
                emitterNode[node.name] = *floatVal;
            } else if(auto const *stringVal{ std::get_if<std::string>(&node.value) }) {
                emitterNode[node.name] = *stringVal;
            } else if(auto const *children{ std::get_if<std::vector<Node>>(&node.value) }; children != nullptr && !children->empty()) {
                YAML::Node childNode{};
                for(auto const &child : *children) {
                    emittNode(childNode, child);
                }
                emitterNode[node.name] = childNode;
            }
        }

        Node buildNode(YAML::Node const &node) {
            Node outNode{};

            for(YAML::const_iterator it{ node.begin() }; it != node.end(); ++it) {
                std::string const name{ it->first.as<std::string>() };
                if(it->second.IsScalar()) {
                    if(it->second.Tag() == "?") {
                        outNode[name] = it->second.as<float>();
                    } else {
                        outNode[name] = it->second.as<std::string>();
                    }
                } else {
                    outNode[name] = buildNode(it->second);
                }
            }

            return outNode;
        }

        namespace v1 {
            std::string emitt(Node const &rootNode) {
                YAML::Node emitterNode{};

                emitterNode["type"]    = "yaml";
                emitterNode["version"] = 1;

                if(auto const *children{ std::get_if<std::vector<Node>>(&rootNode.value) }; children != nullptr) {
                    for(auto const &child : *children) {
                        emittNode(emitterNode, child);
                    }
                }

                YAML::Emitter emitter{};
                emitter << emitterNode;

                std::stringstream stream;
                stream << emitter.c_str();

                return stream.str();
            }

            Node build(YAML::Node const &file) {
                Node deserialisedFile{};

                for(YAML::const_iterator it{ file.begin() }; it != file.end(); ++it) {
                    std::string const name{ it->first.as<std::string>() };
                    //Skip the type and version as these will have been checked
                    if(name == "type" || name == "version") {
                        continue;
                    }

                    if(it->second.IsScalar()) {
                        if(it->second.Tag() == "?") {
                            deserialisedFile[name] = it->second.as<float>();
                        } else {
                            deserialisedFile[name] = it->second.as<std::string>();
                        }
                    } else {
                        deserialisedFile[name] = buildNode(it->second);
                    }
                }

                return deserialisedFile;
            }
        }
    }

    std::string emittYaml(Node const &node) {
        return v1::emitt(node);
    }

    Expected<Node, LoadError> loadYaml(std::filesystem::path const &filePath) {
        YAML::Node file{};
        try {
            file = YAML::LoadFile(filePath.string());
        } catch(YAML::BadFile e) {
            return Unexpected{ LoadError::BadFile };
        }

        if(file["type"].as<std::string>() != "yaml") {
            return Unexpected{ LoadError::WrongType };
        }

        int32_t const version{ file["version"].as<int32_t>() };
        switch(version) {
            case 1:
                return v1::build(file);
            default:
                return Unexpected{ LoadError::WrongVersion };
        }
    }
}