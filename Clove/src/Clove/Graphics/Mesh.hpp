#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class VertexBuffer;
	class IndexBuffer;
	class Material;
	class Shader;
	class VertexBufferData;
	
	class Mesh{
		//VARIABLES
	private:
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<Material> material;
		std::shared_ptr<Shader> shader;//Temp????

		//FUNCTIONS
	public:
		Mesh();
		Mesh(const Mesh& other);
		Mesh& operator=(const Mesh& other);
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

		Mesh(std::string filePath);
		Mesh(const VertexBufferData& vbData, const std::vector<uint32>& indices, ShaderStyle shaderStyle);

		void setMaterial(const std::shared_ptr<Material>& material);
		const std::shared_ptr<Material>& getMaterial() const;

		uint32 getIndexCount();

		void bind();
	};
}
