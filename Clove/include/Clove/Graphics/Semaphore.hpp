#pragma once

namespace clv::gfx {
    /**
     * @brief A Semaphore is a synchronisation object that is used internally by the graphics API.
     * @details Semaphores can be used to insert dependencies between GraphicsQueues.
     */
    class Semaphore {
        //FUNCTIONS
    public:
        virtual ~Semaphore() = default;
    };
}