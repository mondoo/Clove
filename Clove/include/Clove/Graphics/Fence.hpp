#pragma once

namespace clv::gfx {
    /**
     * @brief A Fence is a synchronisation object that can be accessed from the application itself.
     * @details A Fence is best used when the application needs to be synchronised with rendering
     * operations.
     */
    class Fence {
        //TYPES
    public:
        struct Descriptor {
            bool signaled{ false };
        };

        //FUNCTIONS
    public:
        virtual ~Fence() = default;

        /**
         * @brief Makes the current thread wait until the Fence has been signaled
         */
        virtual void wait() = 0;
        virtual void reset() = 0;
    };
}