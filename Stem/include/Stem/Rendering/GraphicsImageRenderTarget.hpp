#pragma once

#include "Stem/Rendering/RenderTarget.hpp"

#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>

#include <queue>

namespace clv::gfx{
    class Fence;
    class Semaphore;
    class GraphicsQueue;
    class TransferQueue;
    class TransferCommandBuffer;
    class GraphicsBuffer;
}

namespace garlic::inline stem {
    /**
     * @brief A RenderTarget backed by a GraphicsImages.
     */
    class GraphicsImageRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        clv::gfx::GraphicsImage::Descriptor imageDescriptor;

        std::shared_ptr<clv::gfx::GraphicsQueue> graphicsQueue;
        std::shared_ptr<clv::gfx::TransferQueue> transferQueue;
        std::shared_ptr<clv::gfx::TransferCommandBuffer> transferCommandBuffer;

        std::shared_ptr<clv::gfx::Semaphore> renderFinishedSemaphore;
        std::shared_ptr<clv::gfx::Fence> frameInFlight;

        std::shared_ptr<clv::gfx::GraphicsImage> renderTargetImage;
        std::shared_ptr<clv::gfx::GraphicsImageView> renderTargetView;
        std::shared_ptr<clv::gfx::GraphicsBuffer> renderTargetBuffer;

        //FUNCTIONS
    public:
        GraphicsImageRenderTarget() = delete;
        GraphicsImageRenderTarget(clv::gfx::GraphicsImage::Descriptor imageDescriptor);

        GraphicsImageRenderTarget(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget(GraphicsImageRenderTarget &&other) noexcept;

        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget &&other) noexcept;

        ~GraphicsImageRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(size_t const frameId) override;

        void submit(uint32_t imageIndex, size_t const frameId, clv::gfx::GraphicsSubmitInfo submission) override;

        clv::gfx::GraphicsImage::Format getImageFormat() const override;
        clv::mth::vec2ui getSize() const override;

        std::vector<std::shared_ptr<clv::gfx::GraphicsImageView>> getImageViews() const override;

        void resize(clv::mth::vec2ui size);

        /**
         * @brief Returns a buffer containing the data of a recently written to image.
         */
        std::shared_ptr<clv::gfx::GraphicsBuffer> getNextReadyBuffer();

    private:
        void createImages();
    };
}