#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class Semaphore;
    class Swapchain;

    struct PresentInfo {
        std::vector<std::shared_ptr<Semaphore>> waitSemaphores;

        std::shared_ptr<Swapchain> swapChain;
        uint32_t imageIndex;
    };
}

namespace clv::gfx {
    /**
	 * @brief Presents a swapchain. Displaying it's contents on the Window / Surface it represents.
     * @details Note that unlike the other queues, a PresentQueue does not need a command buffer.
	 */
    class PresentQueue {
        //FUNCTIONS
    public:
        virtual ~PresentQueue() = default;

        virtual Result present(const PresentInfo& presentInfo) = 0;
    };
}