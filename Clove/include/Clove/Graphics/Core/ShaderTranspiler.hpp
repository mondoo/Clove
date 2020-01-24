#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::gfx{
	//TODO: Rename to shader type
	enum class ShaderOutputType{
		GLSL,
		MSL
	};
}

namespace clv::gfx::ShaderTranspiler{
	//TODO: Rename to transpile
	std::string compile(const std::string& source, ShaderType type, ShaderOutputType outputType);
}
