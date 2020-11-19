#pragma once

namespace clv::gfx {
    /**
     * @brief A Semaphore is a synchronisation object that is used to synchronise queue operations.
     */
    class Semaphore {
        //FUNCTIONS
    public:
        virtual ~Semaphore() = default;

        /**
         * @brief Makes the current thread wait until the Semaphore has been signaled.
         */
        virtual void wait() = 0;
        /**
         * @brief Signals the Semaphore.
         */
        virtual void signal() = 0;
    };
}