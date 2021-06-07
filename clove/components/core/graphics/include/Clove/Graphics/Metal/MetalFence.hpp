#pragma once

#include "Clove/Graphics/GhaFence.hpp"

#include <MetalKit/MetalKit.h>

namespace garlic::clove {
	class MetalFence : public GhaFence {
		//VARIABLES
	private:
		//In metal fences and semaphores are swapped. So a semaphore synchronises between CPU and GPU
		dispatch_semaphore_t semaphore{ nullptr };
		
		//FUNCTIONS
	public:
		MetalFence() = delete;
		MetalFence(dispatch_semaphore_t semaphore);
		
		MetalFence(MetalFence const &other) = delete;
		MetalFence(MetalFence &&other) noexcept;
		
		MetalFence &operator=(MetalFence const &other) = delete;
		MetalFence &operator=(MetalFence &&other) noexcept;
		
		~MetalFence();
		
		void wait() override;
		void reset() override;
		
		inline dispatch_semaphore_t getSemaphore() const;
	};
}

#include "MetalFence.inl"
