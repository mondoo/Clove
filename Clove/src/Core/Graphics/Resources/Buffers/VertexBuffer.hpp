#pragma once

#include "Core/Graphics/Resources/Buffer.hpp"

#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	class Shader;

	struct VertexBufferDescriptor{
		VertexLayout layout;
		std::shared_ptr<Shader> shader;
		uint32 elementSize = 0;
		uint32 bufferSize = 0;
	};
}

namespace clv::gfx{
	class VertexBuffer : public Buffer{
		//FUNCTIONS
	public:
		virtual void setData(const VertexBufferData& data) = 0;
	};
}

