#include "Clove/Graphics/Metal/Resources/MTLBuffer.hpp"

namespace clv::gfx::mtl{
	MTLBuffer::MTLBuffer(id<MTLDevice> mtlDevice, const BufferDescriptor& descriptor, const void* data)
		: descriptor(descriptor){
		mtlBuffer = [mtlDevice newBufferWithLength:descriptor.bufferSize options:0];
		if(data != nullptr){
			memcpy([mtlBuffer contents], data, descriptor.bufferSize);
		}
	}
	
	MTLBuffer::MTLBuffer(MTLBuffer&& other) noexcept = default;
	
	MTLBuffer& MTLBuffer::operator=(MTLBuffer&& other) noexcept = default;
	
	MTLBuffer::~MTLBuffer(){
		[mtlBuffer release];
	}
	
	const BufferDescriptor& MTLBuffer::getDescriptor() const{
		return descriptor;
	}

	void MTLBuffer::updateData(const void* data){
		memcpy([mtlBuffer contents], data, descriptor.bufferSize);
	}

	id<MTLBuffer> MTLBuffer::getMTLBuffer() const{
		return mtlBuffer;
	}
}
