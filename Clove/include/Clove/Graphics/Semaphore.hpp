#pragma once

namespace clv::gfx {
    /**
     * @brief A Semaphore is a synchronisation object that is used to synchronise queues between each other.
     */
    class Semaphore {
        //FUNCTIONS
    public:
        virtual ~Semaphore() = default;
    };
}