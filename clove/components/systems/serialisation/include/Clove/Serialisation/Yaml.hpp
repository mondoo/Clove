#pragma once

#include "Clove/Serialisation/Node.hpp"

#include <string>
#include <filesystem>

namespace garlic::clove{
    std::string emittYaml(serialiser::Node const &node);

    serialiser::Node loadYaml(std::filesystem::path const &filePath);
}