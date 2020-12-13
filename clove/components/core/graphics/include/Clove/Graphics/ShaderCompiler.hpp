#pragma once

#include "Clove/Graphics/Shader.hpp"

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
    std::vector<std::byte> compileFromFile(std::string_view filePath, ShaderType outputType);
    std::vector<std::byte> compileFromSource(std::span<std::byte const> source, ShaderType outputType);
}
