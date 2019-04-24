#include "clvpch.hpp"
#include "RenderableSceneNode.hpp"

#include "Clove/Rendering/API/VertexArray.hpp"
#include "Clove/Rendering/API/VertexBuffer.hpp"
#include "Clove/Rendering/API/Material.hpp"
#include "Clove/Rendering/API/Shader.hpp"
#include "Clove/Rendering/Renderer.hpp"

namespace clv{
	namespace scene{
		RenderableSceneNode::RenderableSceneNode() = default;

		RenderableSceneNode::RenderableSceneNode(RenderableSceneNode&& other) noexcept = default;

		RenderableSceneNode::~RenderableSceneNode() = default;

		void RenderableSceneNode::setMaterial(std::shared_ptr<Material> inMaterial){
			material = inMaterial;
		}

		void RenderableSceneNode::draw(Renderer& renderer){
			renderer.submit(std::static_pointer_cast<RenderableSceneNode>(shared_from_this()));
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