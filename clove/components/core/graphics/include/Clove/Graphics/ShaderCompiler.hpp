#pragma once

#include "Clove/Graphics/Shader.hpp"

#include <Clove/Expected.hpp>
#include <cstddef>
#include <filesystem>
#include <span>
#include <string_view>
#include <unordered_map>
#include <vector>

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
     * @details All includes will be searched for relatively.
     * @param file Absolute path to the file.
     * @param shaderStage The stage that's being compiled.
     * @param outputType Which type of shader code to compile to.
     * @return Compiled shader byte code.
     */
    Expected<std::vector<uint32_t>, std::runtime_error> compileFromFile(std::filesystem::path const &file, Shader::Stage shaderStage, ShaderType outputType);

    /**
     * @brief Compiles source into shader byte code.
     * @param source std::string_view of the Shader source.
     * @param includeSources std::unordered map of shader source strings to use as includes. Key is the name of the include and value is the string of the include.
     * @param shaderName The name of the shader being compiled. Used for debug purposes, can be empty.
     * @param shaderStage The stage that's being compiled.
     * @param outputType Which type of shader code to compile to.
     * @return Compiled shader byte code.
     */
    Expected<std::vector<uint32_t>, std::runtime_error> compileFromSource(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, Shader::Stage shaderStage, ShaderType outputType);
}
