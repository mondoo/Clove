#include "Clove/Reflection/YamlSerialiser.hpp"

#include <yaml-cpp/yaml.h>
#include <sstream>

namespace garlic::clove {
    std::string YamlSerialiser::emitt() {
        YAML::Node emitterNode{};

        emitterNode["type"] = "yaml";
        emitterNode["version"] = 1;

        emittNode(emitterNode, root);

        YAML::Emitter emitter{};
        emitter << emitterNode;

        std::stringstream stream;
        stream << emitter.c_str();

        return stream.str();
    }
}