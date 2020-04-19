#pragma once

#include "Clove/Graphics/FactoryChild.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/VertexLayout.hpp"

namespace clv::gfx {
	struct ShaderDescriptor {
		ShaderStage stage;
	};
}

namespace clv::gfx {
	class Shader : public FactoryChild {
		//FUNCTIONS
	public:
		virtual ~Shader() = default;

		virtual const ShaderDescriptor& getDescriptor() const = 0;
	};
}