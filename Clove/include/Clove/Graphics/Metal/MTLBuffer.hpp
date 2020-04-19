#pragma once

#include "Clove/Graphics/Buffer.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl{
	class MTLBuffer : public Buffer{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		BufferDescriptor descriptor;
		
		id<MTLBuffer> mtlBuffer;
		
		//FUNCTIONS
	public:
		MTLBuffer() = delete;
		MTLBuffer(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, const BufferDescriptor& descriptor, const void* data);
		
		MTLBuffer(const MTLBuffer& other) = delete;
		MTLBuffer(MTLBuffer&& other) noexcept;
		
		MTLBuffer& operator=(const MTLBuffer& other) = delete;
		MTLBuffer& operator=(MTLBuffer&& other) noexcept;
		
		virtual ~MTLBuffer();
		
		virtual const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		virtual const BufferDescriptor& getDescriptor() const override;

		virtual void updateData(const void* data) override;

		id<MTLBuffer> getMTLBuffer() const;
	};
}
