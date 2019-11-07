#pragma once

#include "Core/Graphics/Resources/Buffer.hpp"

namespace clv::gfx{
	class Shader;

	struct VertexBufferDescriptor{
		VertexBufferData bufferData;
		std::shared_ptr<Shader> shader;
	};
}

namespace clv::gfx{
	class VertexBuffer : public Buffer{
		//FUNCTIONS
	public:
		//TODO
	};
}

