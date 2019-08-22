#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/MaterialInstance.hpp"

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
		MaterialInstance materialInstance;

		//FUNCTIONS
	public:
		Mesh() = delete;
		Mesh(const Mesh& other);
		Mesh& operator=(const Mesh& other);
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

		Mesh(std::string filePath, MaterialInstance materialInstance);
		Mesh(const VertexBufferData& vbData, const std::vector<uint32>& indices, MaterialInstance materialInstance);

		MaterialInstance& getMaterialInstance();

		uint32 getIndexCount();

		void bind();
	};
}
