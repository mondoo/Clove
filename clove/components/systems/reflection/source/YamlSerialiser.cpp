#include "Clove/Reflection/YamlSerialiser.hpp"

#include <yaml-cpp/yaml.h>
#include <sstream>

namespace garlic::clove {
    std::string YamlSerialiser::emitt() {
        YAML::Emitter emitter;
        emitter << YAML::BeginMap << root.name << YAML::EndMap;

        std::stringstream stream;
        stream << emitter.c_str();

        return stream.str();
    }
}