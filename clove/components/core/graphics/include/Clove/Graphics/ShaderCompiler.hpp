#pragma once

#include "Clove/Graphics/Shader.hpp"

#include <string_view>

namespace garlic::clove {
    enum class ShaderType {
        SPIRV,
        HLSL,
        MSL,
    };
}

namespace garlic::clove::ShaderCompiler {
    std::string compileFromFile(std::string_view filePath, Shader::Stage stage, ShaderType outputType);
    std::string compileFromBytes(char const *bytes, std::size_t const size, Shader::Stage stage, ShaderType outputType);
    std::string compileFromSource(std::string_view source, Shader::Stage stage, ShaderType outputType);
}
