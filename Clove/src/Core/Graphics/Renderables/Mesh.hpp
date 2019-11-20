#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/MaterialInstance.hpp"
#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	class Buffer;
	class VertexLayout;
	class VertexBufferData;
}

namespace clv::gfx{
	class Mesh{
		//VARIABLES
	private:
		std::shared_ptr<Buffer> vertexBuffer;
		std::shared_ptr<Buffer> indexBuffer;
		MaterialInstance materialInstance;

		VertexBufferData loadedBufferData;
		std::vector<uint32> indices;

		//FUNCTIONS
	public:
		Mesh() = delete;
		Mesh(std::string filePath, MaterialInstance materialInstance);
		Mesh(const VertexBufferData& vbData, const std::vector<uint32>& indices, MaterialInstance materialInstance);
		
		Mesh(const Mesh& other);
		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(const Mesh& other);
		Mesh& operator=(Mesh&& other) noexcept;

		~Mesh();

		MaterialInstance& getMaterialInstance();

		uint32 getIndexCount();

		std::shared_ptr<Buffer> getVertexBufferForLayout(const VertexLayout& layout);
		std::shared_ptr<Buffer> getIndexBuffer();

	private:
		void initialiseVertexBuffer(const VertexBufferData& vertexArray);
		void initialiseIndexBuffer(const std::vector<uint32>& indices);
	};
}
