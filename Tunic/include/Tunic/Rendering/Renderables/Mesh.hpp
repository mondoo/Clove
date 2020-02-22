#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Tunic/Rendering/MaterialInstance.hpp"
#include "Clove/Graphics/Core/VertexLayout.hpp"

namespace clv::gfx{
	class Buffer;
	class VertexLayout;
	class VertexBufferData;
	class CommandBuffer;
}

namespace tnc::rnd{
	class Mesh{
		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::Buffer> vertexBuffer;
		std::shared_ptr<clv::gfx::Buffer> indexBuffer;
		MaterialInstance materialInstance;

		clv::gfx::VertexBufferData loadedBufferData;
		std::vector<uint32_t> indices;

		//FUNCTIONS
	public:
		Mesh() = delete;
		Mesh(std::string_view filePath, MaterialInstance materialInstance);
		Mesh(const clv::gfx::VertexBufferData& vbData, const std::vector<uint32_t>& indices, MaterialInstance materialInstance);
		
		Mesh(const Mesh& other);
		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(const Mesh& other);
		Mesh& operator=(Mesh&& other) noexcept;

		~Mesh();

		MaterialInstance& getMaterialInstance();

		uint32_t getIndexCount();

		void draw(clv::gfx::CommandBuffer& commandBuffer, const clv::gfx::VertexLayout& layout);

	private:
		void initialiseVertexBuffer(const clv::gfx::VertexBufferData& vertexArray);
		void initialiseIndexBuffer(const std::vector<uint32_t>& indices);
	};
}
