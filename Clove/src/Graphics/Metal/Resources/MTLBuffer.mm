#include "Clove/Platform/Mac/CloveMac.h"
#include "Clove/Graphics/Metal/Resources/MTLBuffer.hpp"

namespace clv::gfx::mtl{
	MTLBuffer::MTLBuffer(id<MTLDevice> mtlDevice, const BufferDescriptor& descriptor, const void* data)
		: descriptor(descriptor){
		mtlBuffer = [mtlDevice newBufferWithBytes:data length:descriptor.bufferSize options:0];
	}
	
	MTLBuffer::MTLBuffer(MTLBuffer&& other) noexcept = default;
	
	MTLBuffer& MTLBuffer::operator=(MTLBuffer&& other) noexcept = default;
	
	MTLBuffer::~MTLBuffer(){
		[mtlBuffer release];
	}
	
	const BufferDescriptor& MTLBuffer::getDescriptor() const{
		return descriptor;
	}
}
