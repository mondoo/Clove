#pragma once

#include "Clove/Rendering/API/RenderingTypes.hpp"

namespace clv{
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Material;

	class Shader;

	class Renderable{
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
		Renderable();
		Renderable(const Renderable& other) = delete;
		Renderable(Renderable&& other) noexcept;
		
		virtual ~Renderable();

		inline unsigned int getIndexBufferCount() const;
		inline const ShaderType getShaderType() const;

		void bind(Shader& shader);
		void unbind();

		Renderable& operator=(Renderable&& other) noexcept;
	};
}

#include "Renderable.inl"