#include "Clove/Rendering/API/IndexBuffer.hpp"
#include "Clove/Rendering/API/Material.hpp"

namespace clv{
	namespace scene{
		inline unsigned int RenderableSceneNode::getIndexBufferCount() const{
			return indexBuffer->getCount();
		}

		inline const ShaderType RenderableSceneNode::getShaderType() const{
			return material->getShaderType();
		}
	}
}