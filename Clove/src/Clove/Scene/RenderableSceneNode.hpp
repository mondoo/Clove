#pragma once

#include "Clove/Scene/SceneNode.hpp"

#include "Clove/Rendering/API/RenderingTypes.hpp"

namespace clv{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Material;

	class Shader;

	namespace scene{
		class RenderableSceneNode : public SceneNode{
			//VARIABLES
		protected:
			std::vector<float> vertexData;
			std::vector<unsigned int> indices;

			std::unique_ptr<VertexArray> vertexArray;
			std::unique_ptr<VertexBuffer> vertexBuffer;
			std::unique_ptr<IndexBuffer> indexBuffer;

			std::shared_ptr<Material> material;

			//FUNCTIONS
		public:
			RenderableSceneNode();
			RenderableSceneNode(const RenderableSceneNode& other) = delete; //TODO: is there a way to not delete this?
			RenderableSceneNode(RenderableSceneNode&& other) noexcept;

			virtual ~RenderableSceneNode();

			CLV_API void setMaterial(std::shared_ptr<Material> inMaterial);

			inline unsigned int getIndexBufferCount() const;
			inline const ShaderType getShaderType() const;

			void bind(Shader& shader);
			void unbind();

			RenderableSceneNode& operator=(RenderableSceneNode&& other) noexcept;
		};
	}
}

#include "RenderableSceneNode.inl"