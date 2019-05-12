#include "GraphicsAPI/GL/OLD/IndexBuffer.hpp"
#include "GraphicsAPI/GL/OLD/Material.hpp"

namespace clv{
	namespace scene{
		inline unsigned int RenderableSceneNode::getIndexBufferCount() const{
			return indexBuffer->getCount();
		}

		inline const gfx::ShaderType RenderableSceneNode::getShaderType() const{
			return material->getShaderType();
		}
	}
}