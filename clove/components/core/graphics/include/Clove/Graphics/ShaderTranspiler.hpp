#pragma once

#include "Clove/Graphics/Shader.hpp"

#include <string_view>

namespace garlic::clove {
    enum class ShaderType {
        GLSL,
        MSL
    };
}

namespace garlic::clove::ShaderTranspiler {
    std::string transpileFromFile(std::string_view filePath, Shader::Stage stage, ShaderType outputType);
    std::string transpileFromBytes(char const *bytes, std::size_t const size, Shader::Stage stage, ShaderType outputType);
    std::string transpileFromSource(std::string_view source, Shader::Stage stage, ShaderType outputType);
}
