#pragma once

#include "Clove/Graphics/Shader.hpp"

#include <cstddef>
#include <string_view>
#include <vector>
#include <filesystem>

namespace garlic::clove {
    enum class ShaderType {
        SPIRV,
        HLSL,
        MSL,
    };
}

namespace garlic::clove::ShaderCompiler {
    /**
     * @brief Compiles file into shader byte code. 
     * @param file Absolute path to the file.
     * @param shaderStage The stage that's being compiled.
     * @param outputType Which type of shader code to compile to.
     * @return Compiled shader byte code.
     */
    std::vector<uint32_t> compileFromFile(std::filesystem::path const &file, Shader::Stage shaderStage, ShaderType outputType);
    std::vector<uint32_t> compileFromSource(std::string_view source, Shader::Stage shaderStage, ShaderType outputType);
}
