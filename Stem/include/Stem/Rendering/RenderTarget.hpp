#pragma once

#include "Clove/Graphics/GraphicsImage.hpp"

#include <Root/Expected.hpp>
#include <Root/Delegate/MultiCastDelegate.hpp>

namespace clv::gfx {
    class Semaphore;
    class FrameBuffer;
    class GraphicsSubmitInfo;
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
         * @param frameId Which frame is currently being rendered. This 
         * number will be from 0 - N-1 where N is the total number of frames in flight.
         * @return Returns the image index for the getImageViews array.
         */
        virtual Expected<uint32_t, std::string> aquireNextImage(size_t const frameId) = 0;

        /**
         * @brief Submit graphics command buffers to be presented on this RenderTarget.
         * @param imageIndex The image index of the getImageViews array this submission is for.
         * @param frameId Which frame is currently being rendered. This 
         * number will be from 0 - N-1 where N is the total number of frames in flight.
         * @param primarySubmission The primary submission. i.e. The command buffer for the final colour
         * @param secondarySubmissions Any graphics submissions the primarySubmission depends on. i.e. Shadow map generation.
         */
        virtual void submit(uint32_t imageIndex, size_t const frameId, clv::gfx::GraphicsSubmitInfo primarySubmission, std::vector<clv::gfx::GraphicsSubmitInfo> secondarySubmissions) = 0;

        virtual clv::gfx::GraphicsImage::Format getImageFormat() const = 0;
        virtual clv::mth::vec2ui getSize() const                       = 0;

        virtual std::vector<std::shared_ptr<clv::gfx::GraphicsImageView>> getImageViews() const = 0;
    };
}