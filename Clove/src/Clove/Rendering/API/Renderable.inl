#include "Clove/Rendering/API/IndexBuffer.hpp"

namespace clv{
	inline unsigned int Renderable::getIndexBufferCount() const{
		return indexBuffer->getCount();
	}

	inline ShaderType Renderable::getShaderType() const{
		return shaderType;
	}
}