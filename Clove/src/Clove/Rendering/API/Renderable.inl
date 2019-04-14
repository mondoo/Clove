#include "Clove/Rendering/API/IndexBuffer.hpp"
#include "Clove/Rendering/API/Material.hpp"

namespace clv{
	inline unsigned int Renderable::getIndexBufferCount() const{
		return indexBuffer->getCount();
	}

	inline const ShaderType Renderable::getShaderType() const{
		return material->getShaderType();
	}
}