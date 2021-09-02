#pragma once

#include <cstddef>

namespace clove {
    class GhaFactory;
}

namespace clove {
    /**
     * @brief Represents the actual hardware used for rendering. Can be quieried for info about the device.
     */
    class GhaDevice {
        //TYPES
    public:
        /**
         * @brief Contains information about the limits of the device used by the GHA.
         */
        struct Limits {
            size_t minUniformBufferOffsetAlignment{ 0 }; /**< If multiple descriptor bindings exist in a single GhaBuffer then this is the minimum alignment that each binding needs to have within that buffer. */
        };

        //FUNCTIONS
    public:
        virtual ~GhaDevice() = default;

        /**
         * @brief Returns a pointer to the factory object. The lifetime of the 
         * factory is tied to the lifetime of this device.
         * @return 
         */
        virtual GhaFactory *getGraphicsFactory() const = 0;

        /**
         * @brief Stalls the current thread until the device is idle.
         */
        virtual void waitForIdleDevice() = 0;

        virtual Limits getLimits() const = 0;
    };
}
