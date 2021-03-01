#pragma once

#include "Clove/Serialisation/Node.hpp"

#include <Clove/Expected.hpp>
#include <string>
#include <filesystem>

namespace garlic::clove{
    enum class LoadError {
        BadFile,
        WrongType,
        WrongVersion,
    };

    std::string emittYaml(serialiser::Node const &node);

    Expected<serialiser::Node, LoadError> loadYaml(std::filesystem::path const &filePath);
}