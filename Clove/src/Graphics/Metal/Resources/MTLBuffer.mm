#include "Clove/Graphics/Metal/Resources/MTLBuffer.hpp"

namespace clv::gfx::mtl{
	MTLBuffer::MTLBuffer(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, const BufferDescriptor& descriptor, const void* data)
		: factory(std::move(factory))
		, descriptor(descriptor){
		mtlBuffer = [mtlDevice newBufferWithLength:descriptor.bufferSize options:0];
		updateData(data);
	}
	
	MTLBuffer::MTLBuffer(MTLBuffer&& other) noexcept = default;
	
	MTLBuffer& MTLBuffer::operator=(MTLBuffer&& other) noexcept = default;
	
	MTLBuffer::~MTLBuffer(){
		[mtlBuffer release];
	}

	const std::shared_ptr<GraphicsFactory>& MTLBuffer::getFactory() const{
		return factory;
	}
	
	const BufferDescriptor& MTLBuffer::getDescriptor() const{
		return descriptor;
	}

	void MTLBuffer::updateData(const void* data){
		if(data != nullptr){
			memcpy([mtlBuffer contents], data, descriptor.bufferSize);
		}
	}

	id<MTLBuffer> MTLBuffer::getMTLBuffer() const{
		return mtlBuffer;
	}
}
