#pragma once

#include "Clove/Graphics/Result.hpp"

#include <vector>

namespace clove {
    class GhaSemaphore;
    class GhaSwapchain;

    struct PresentInfo {
        std::vector<GhaSemaphore const *> waitSemaphores;

        GhaSwapchain *swapChain{ nullptr };
        uint32_t imageIndex;
    };
}

namespace clove {
    /**
	 * @brief Presents a swapchain. Displaying it's contents on the Window / Surface it represents.
     * @details Note that unlike the other queues, a GhaPresentQueue does not need a command buffer.
	 */
    class GhaPresentQueue {
        //FUNCTIONS
    public:
        virtual ~GhaPresentQueue() = default;

        virtual Result present(PresentInfo const &presentInfo) = 0;
    };
}
