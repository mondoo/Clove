#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Shader.hpp"

namespace clv::gfx{
	enum class ShaderType{
		GLSL,
		MSL
	};
}

namespace clv::gfx::ShaderTranspiler{
	std::string transpileFromFile(std::string_view filePath, Shader::Stage stage, ShaderType outputType);
	std::string transpileFromBytes(const char* bytes, const std::size_t size, Shader::Stage stage, ShaderType outputType);
	std::string transpileFromSource(std::string_view source, Shader::Stage stage, ShaderType outputType);
}
