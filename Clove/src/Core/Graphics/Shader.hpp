#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	struct ShaderDescriptor{
		ShaderStyle style;
	};

	struct ShaderReflectionData{
		VertexLayout vertexBufferLayout;
	};
}

namespace clv::gfx{
	class Shader{
		//FUNCTIONS
	public:
		virtual const ShaderDescriptor& getDescriptor() const = 0;
		virtual ShaderReflectionData getReflectionData() const = 0;
	};
}