#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::gfx{
	enum class ShaderType{
		GLSL,
		MSL
	};
}

namespace clv::gfx::ShaderTranspiler{
	//TODO: Take the file path
	std::string compile(const std::string& source, ShaderStage type, ShaderType outputType);
}
