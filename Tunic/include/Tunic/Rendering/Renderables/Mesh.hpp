#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Tunic/Rendering/MaterialInstance.hpp"
#include "Clove/Graphics/Core/VertexLayout.hpp"

namespace clv::gfx{
	class Buffer;
	class VertexLayout;
	class VertexBufferData;
}

namespace tnc::rnd{
	class Mesh{
		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::Buffer> vertexBuffer;
		std::shared_ptr<clv::gfx::Buffer> indexBuffer;
		MaterialInstance materialInstance;

		clv::gfx::VertexBufferData loadedBufferData;
		std::vector<clv::uint32> indices;

		//FUNCTIONS
	public:
		Mesh() = delete;
		Mesh(std::string filePath, MaterialInstance materialInstance);
		Mesh(const clv::gfx::VertexBufferData& vbData, const std::vector<clv::uint32>& indices, MaterialInstance materialInstance);
		
		Mesh(const Mesh& other);
		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(const Mesh& other);
		Mesh& operator=(Mesh&& other) noexcept;

		~Mesh();

		MaterialInstance& getMaterialInstance();

		clv::uint32 getIndexCount();

		std::shared_ptr<clv::gfx::Buffer> getVertexBufferForLayout(const clv::gfx::VertexLayout& layout);
		std::shared_ptr<clv::gfx::Buffer> getIndexBuffer();

	private:
		void initialiseVertexBuffer(const clv::gfx::VertexBufferData& vertexArray);
		void initialiseIndexBuffer(const std::vector<clv::uint32>& indices);
	};
}
