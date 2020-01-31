#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/Core/VertexLayout.hpp"

namespace clv::gfx{
	struct ShaderDescriptor{
		ShaderStage stage;
	};
}

namespace clv::gfx{
	class Shader{
		//FUNCTIONS
	public:
		virtual ~Shader() = default;

		virtual const ShaderDescriptor& getDescriptor() const = 0;
	};
}