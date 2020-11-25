#pragma once

namespace garlic::clove {
    /**
     * @brief A Fence is a synchronisation object that can be used to synchronise the application
     * with a graphics operation.
     */
    class Fence {
        //TYPES
    public:
        struct Descriptor {
            bool signaled{ false }; /**< If true, will initialise the Fence as signaled. */
        };

        //FUNCTIONS
    public:
        virtual ~Fence() = default;

        /**
         * @brief Makes the current thread wait until the Fence has been signaled.
         */
        virtual void wait() = 0;
        /**
         * @brief Resets the Fence back into an unsignaled state.
         */
        virtual void reset() = 0;
    };
}