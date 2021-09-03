#pragma once

#include <cstddef>
#include <string>
#include <optional>

namespace clove {
    class GhaFactory;

    struct Version{
        uint32_t major{ 0 };
        uint32_t minor{ 0 };
        uint32_t patch{ 0 };
    };
}

namespace clove {
    /**
     * @brief Represents the actual hardware used for rendering. Can be quieried for info about the device.
     */
    class GhaDevice {
        //TYPES
    public:
        /**
         * @brief Contains info about the device itself that the GHA is using.
         */
        struct Info {
            std::string ApiName{};
            std::string deviceName{};
            std::optional<Version> driverVersion{}; /**< Not provided by some APIs. */
            std::optional<Version> ApiVersion{};    /**< Not provided by some APIs. */
        };

        /**
         * @brief Contains information about the limits of the device used by the GHA.
         */
        struct Limits {
            size_t minUniformBufferOffsetAlignment{ 0 }; /**< The minimum offset allowed (if greater than 0) when calling GhaDescriptorSet::map with a GhaBuffer. */
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

        virtual Info getInfo() const     = 0;
        virtual Limits getLimits() const = 0;
    };
}
