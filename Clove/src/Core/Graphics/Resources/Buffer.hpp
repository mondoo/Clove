#pragma once

namespace clv::gfx{
	enum class BufferType{
		IndexBuffer,
		VertexBuffer,
		ShaderResourceBuffer
	};

	enum class BufferUsage{
		Default,
		Dynamic
	};

	struct BufferDescriptor{
		uint32 elementSize = 0;
		uint32 bufferSize = 0;
		BufferType bufferType;
		BufferUsage bufferUsage;
	};
}

namespace clv::gfx{
	class Buffer{
		//FUNCTIONS
	public:
		virtual ~Buffer() = default;

		virtual const BufferDescriptor& getDescriptor() const = 0;
	};
}