#pragma once

#include "Clove/Graphics/GraphicsImage.hpp"

#include <Root/Expected.hpp>
#include <Root/Delegate/MultiCastDelegate.hpp>

namespace clv::gfx {
    class Semaphore;
}

namespace garlic::inline stem {
    /**
     * @brief Contains an array of 1-N images that can be rendered to.
     */
    class RenderTarget {
        //VARIABLES
    public:
        //Gets called when the properties of this RenderTarget change. Usually requiring rendering primitives to be recreated.
        MultiCastDelegate<void()> onPropertiesChanged{};

        //FUNCTIONS
    public:
        RenderTarget() = default;

        RenderTarget(RenderTarget const &other) = delete;
        RenderTarget(RenderTarget &&other) noexcept = default;

        RenderTarget &operator=(RenderTarget const &other) = delete;
        RenderTarget &operator=(RenderTarget &&other) noexcept = default;

        virtual ~RenderTarget() = default;

        /**
         * @brief Aquire the next available image that can be rendered to.
         * @param availableSemaphore A Semaphore that will be signaled when the image is ready. Can be nullptr
         * @return Returns the image index.
         */
        virtual Expected<uint32_t, std::string> aquireNextImage(std::shared_ptr<clv::gfx::Semaphore> availableSemaphore) = 0;

        /**
         * @brief Tells the RenderTarget that all operations on the image are completed and it is ready to be presented.
         * @param imageIndex The index of the image that can be presented.
         * @param waitSemaphores Semaphores for the RenderTarget to wait on before beginning presentation logic.
         */
        virtual void present(uint32_t imageIndex, std::vector<std::shared_ptr<clv::gfx::Semaphore>> waitSemaphores) = 0;

        virtual clv::gfx::GraphicsImage::Format getImageFormat() const = 0;
        virtual clv::mth::vec2ui getSize() const                       = 0;

        virtual std::vector<std::shared_ptr<clv::gfx::GraphicsImageView>> getImageViews() const = 0;
    };
}