#include "clvpch.hpp"
#include "RenderableSceneNode.hpp"

#include "GraphicsAPI/GL/OLD/VertexArray.hpp"
#include "GraphicsAPI/GL/OLD/VertexBuffer.hpp"
#include "GraphicsAPI/GL/OLD/Material.hpp"
#include "GraphicsAPI/GL/OLD/Shader.hpp"
#include "Clove/Graphics/Renderer.hpp"

namespace clv{
	namespace scene{
		RenderableSceneNode::RenderableSceneNode() = default;

		RenderableSceneNode::RenderableSceneNode(RenderableSceneNode&& other) noexcept = default;

		RenderableSceneNode::~RenderableSceneNode() = default;

		void RenderableSceneNode::setMaterial(std::shared_ptr<Material> inMaterial){
			material = inMaterial;
		}

		void RenderableSceneNode::bind(Shader& shader){
			vertexArray->bind();
			indexBuffer->bind();
			material->bind(shader);
		}

		void RenderableSceneNode::unbind(){
			vertexArray->unbind();
			indexBuffer->unbind();
			material->unbind();
		}

		RenderableSceneNode& RenderableSceneNode::operator=(RenderableSceneNode&& other) noexcept = default;
	}
}