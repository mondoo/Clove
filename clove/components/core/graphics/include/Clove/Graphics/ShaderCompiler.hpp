#pragma once

#include "Clove/Graphics/Shader.hpp"

#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

namespace garlic::clove {
    enum class ShaderType {
        SPIRV,
        HLSL,
        MSL,
    };
}

namespace garlic::clove::ShaderCompiler {
    std::vector<uint32_t> compileFromFile(std::string_view filePath, Shader::Stage shaderStage, ShaderType outputType);
    std::vector<uint32_t> compileFromSource(std::span<std::byte> source, Shader::Stage shaderStage, ShaderType outputType);
}
