#include "Clove/Graphics/Metal/MetalSwapchain.hpp"

#include "Clove/Graphics/Metal/MetalImage.hpp"
#include "Clove/Graphics/Metal/MetalImageView.hpp"
#include "Clove/Graphics/Metal/MetalLog.hpp"
#include "Clove/Graphics/Metal/MetalSemaphore.hpp"

#include <Clove/Cast.hpp>

namespace clove {
	MetalSwapchain::MetalSwapchain(id<MTLCommandQueue> signalQueue, std::vector<std::unique_ptr<GhaImage>> images, GhaImage::Format imageFormat, vec2ui imageSize)
        : signalQueue{ signalQueue }
		, images{ std::move(images) }
		, imageFormat{ imageFormat }
		, imageSize{ imageSize } {
        for(size_t i{ 0 }; i < this->images.size(); ++i) {
            imageQueue.push(i);
        }
	}
	
	MetalSwapchain::MetalSwapchain(MetalSwapchain &&other) noexcept = default;
	
	MetalSwapchain& MetalSwapchain::operator=(MetalSwapchain &&other) noexcept = default;
	
	MetalSwapchain::~MetalSwapchain() = default;
	
	std::pair<uint32_t, Result> MetalSwapchain::aquireNextImage(GhaSemaphore const *availableSemaphore) {
		//TODO: Handle resizing;
        
		if(imageQueue.empty()) {
			CLOVE_LOG(CloveGhaMetal, LogLevel::Error, "{0} has no available images", CLOVE_FUNCTION_NAME_PRETTY);
			return { -1, Result::Unkown };
		}
		
		uint32_t const availableIndex{ imageQueue.front() };
		imageQueue.pop();
        
        //Because we're simulating the swapchain on the CPU side we need to signal the semaphores immediately.
        //This is a hack solution as we wouldn't want to just make an empty commit that only signals a
        //semaphore. But it'll do for now
        id<MTLCommandBuffer> commandBuffer{ [signalQueue commandBuffer] };
        id<MTLBlitCommandEncoder> encoder{ [commandBuffer blitCommandEncoder] };
        
        [encoder waitForFence:polyCast<MetalSemaphore const>(availableSemaphore)->getFence()];
        
        [encoder endEncoding];
        [commandBuffer commit];
        
		
		return { availableIndex, Result::Success };
	}

	GhaImage::Format MetalSwapchain::getImageFormat() const {
		return imageFormat;
	}
	
	vec2ui MetalSwapchain::getSize() const {
		return imageSize;
	}

	std::vector<GhaImage *> MetalSwapchain::getImages() const {
        std::vector<GhaImage *> ghaImages{};
        ghaImages.reserve(images.size());
        
        for(auto const &image : images) {
            ghaImages.push_back(image.get());
        }
        
		return ghaImages;
	}
	
	void MetalSwapchain::markIndexAsFree(uint32_t index) {
		imageQueue.push(index);
	}
}
