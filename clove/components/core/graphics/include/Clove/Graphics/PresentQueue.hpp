#pragma once

#include "Clove/Graphics/Result.hpp"

#include <memory>
#include <vector>

namespace garlic::clove {
    class Semaphore;
    class Swapchain;

    struct PresentInfo {
        std::vector<std::shared_ptr<Semaphore>> waitSemaphores;

        std::shared_ptr<Swapchain> swapChain;
        uint32_t imageIndex;
    };
}

namespace garlic::clove {
    /**
	 * @brief Presents a swapchain. Displaying it's contents on the Window / Surface it represents.
     * @details Note that unlike the other queues, a PresentQueue does not need a command buffer.
	 */
    class PresentQueue {
        //FUNCTIONS
    public:
        virtual ~PresentQueue() = default;

        virtual Result present(PresentInfo const &presentInfo) = 0;
    };
}