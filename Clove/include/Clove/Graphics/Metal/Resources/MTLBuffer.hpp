#pragma once

#include "Clove/Core/Graphics/Resources/Buffer.hpp"

namespace clv::gfx::mtl{
	class MTLBuffer : public Buffer{
		//VARIABLES
	private:
		BufferDescriptor descriptor;
		
		id<MTLBuffer> mtlBuffer;
		
		//FUNCTIONS
	public:
		MTLBuffer() = delete;
		MTLBuffer(id<MTLDevice> mtlDevice, const BufferDescriptor& descriptor, const void* data);
		
		MTLBuffer(const MTLBuffer& other) = delete;
		MTLBuffer(MTLBuffer&& other) noexcept;
		
		MTLBuffer& operator=(const MTLBuffer& other) = delete;
		MTLBuffer& operator=(MTLBuffer&& other) noexcept;
		
		virtual ~MTLBuffer();
		
		virtual const BufferDescriptor& getDescriptor() const override;
	};
}
