#pragma once

namespace clv::gfx {
    /**
     * @brief A Semaphore is a synchronisation object that is used to synchronise queue operations.
     */
    class Semaphore {
        //FUNCTIONS
    public:
        virtual ~Semaphore() = default;
    };
}