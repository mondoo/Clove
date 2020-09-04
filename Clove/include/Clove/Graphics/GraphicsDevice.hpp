#pragma once

namespace clv::gfx{
    class GraphicsFactory;
}

namespace clv::gfx {
    /**
     * @brief Represents the actual hardware used for rendering. Can be quieried for info about the device.
     */
    class GraphicsDevice {
        //FUNCTIONS
    public:
        virtual ~GraphicsDevice() = default;

        virtual std::shared_ptr<GraphicsFactory> getGraphicsFactory() const = 0;

        /**
         * @brief Stalls the current thread until the device is idle.
         */
        virtual void waitForIdleDevice() = 0;
    };
}