#pragma once

#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/Core/VertexLayout.hpp"
#include "Tunic/Rendering/MaterialInstance.hpp"

namespace clv::gfx {
	class Buffer;
	class VertexLayout;
	class VertexBufferData;
	class CommandBuffer;
}

namespace tnc::rnd {
	class Mesh {
		//VARIABLES
	private:
		std::unordered_map<clv::gfx::VertexLayout, std::shared_ptr<clv::gfx::Buffer>, clv::gfx::VertexLayoutHasher> vertexBufferMap;
		std::shared_ptr<clv::gfx::Buffer> indexBuffer;
		MaterialInstance materialInstance;

		clv::gfx::VertexBufferData loadedBufferData;
		std::vector<uint32_t> indices;

		//FUNCTIONS
	public:
		Mesh() = delete;
		Mesh(const clv::gfx::VertexBufferData& vbData, const std::vector<uint32_t>& indices, MaterialInstance materialInstance);

		Mesh(const Mesh& other);
		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(const Mesh& other);
		Mesh& operator=(Mesh&& other) noexcept;

		~Mesh();

		void setMaterialInstance(MaterialInstance materialInstance);
		MaterialInstance& getMaterialInstance();

		uint32_t getIndexCount();

		void draw(clv::gfx::CommandBuffer& commandBuffer, const clv::gfx::VertexLayout& layout);
	};
}
