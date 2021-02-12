#include "Clove/Serialisation/Yaml.hpp"

#include <sstream>
#include <yaml-cpp/yaml.h>

using namespace garlic::clove::serialiser;

namespace garlic::clove {
    namespace {
        void emittNode(YAML::Node &emitterNode, Node const &node) {
            if(node.getKey().length() <= 0 && node.getChildren().size() <= 0) {
                return;
            }

            if(node.getType() == Node::Type::Scalar) {
                emitterNode[node.getKey()] = node.as<std::string>();
            } else {
                YAML::Node childNode{};
                for(auto const &child : node) {
                    emittNode(childNode, child);
                }
                emitterNode[node.getKey()] = childNode;
            }
        }

        Node buildNode(YAML::Node const &node) {
            Node outNode{};

            for(YAML::const_iterator it{ node.begin() }; it != node.end(); ++it) {
                std::string const name{ it->first.as<std::string>() };
                if(it->second.IsScalar()) {
                    outNode[name] = it->second.as<std::string>();
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

                //Loop through the children so the rootNode isn't added to the file
                for(auto const &child : rootNode) {
                    emittNode(emitterNode, child);
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
                        deserialisedFile[name] = it->second.as<std::string>();
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