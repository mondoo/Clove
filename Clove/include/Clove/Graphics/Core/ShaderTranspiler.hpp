#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::gfx{
	enum class ShaderType{
		GLSL,
		MSL
	};
}

namespace clv::gfx::ShaderTranspiler{
	std::string transpileFromFile(std::string_view filePath, ShaderStage stage, ShaderType outputType);
	std::string transpileFromSource(std::string_view source, ShaderStage stage, ShaderType outputType);
}
