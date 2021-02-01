#pragma once

#include "Clove/Serialisation/Node.hpp"

#include <string>

namespace garlic::clove{
    std::string emittYaml(serialiser::Node const &node);
}