#pragma once

namespace garlic::clove {
    /**
     * @brief A GhaSemaphore is a synchronisation object that is used to synchronise queues between each other.
     */
    class GhaSemaphore {
        //FUNCTIONS
    public:
        virtual ~GhaSemaphore() = default;
    };
}