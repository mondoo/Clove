#pragma once

#include "Clove/Rendering/RenderTarget.hpp"

#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/GraphicsImageView.hpp>

#include <queue>

namespace garlic::clove{
    class Fence;
    class Semaphore;
    class GraphicsQueue;
    class TransferQueue;
    class TransferCommandBuffer;
    class GraphicsBuffer;
    class GraphicsFactory;
}

namespace garlic::clove {
    /**
     * @brief A RenderTarget backed by a GraphicsImages.
     */
    class GraphicsImageRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        GraphicsImage::Descriptor imageDescriptor;

        std::shared_ptr<GraphicsFactory> factory;

        std::shared_ptr<GraphicsQueue> graphicsQueue;
        std::shared_ptr<TransferQueue> transferQueue;
        std::shared_ptr<TransferCommandBuffer> transferCommandBuffer;

        std::shared_ptr<Semaphore> renderFinishedSemaphore;
        std::shared_ptr<Fence> frameInFlight;

        std::shared_ptr<GraphicsImage> renderTargetImage;
        std::shared_ptr<GraphicsImageView> renderTargetView;
        std::shared_ptr<GraphicsBuffer> renderTargetBuffer;

        //FUNCTIONS
    public:
        GraphicsImageRenderTarget() = delete;
        GraphicsImageRenderTarget(GraphicsImage::Descriptor imageDescriptor, std::shared_ptr<GraphicsFactory> factory);

        GraphicsImageRenderTarget(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget(GraphicsImageRenderTarget &&other) noexcept;

        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget &&other) noexcept;

        ~GraphicsImageRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(size_t const frameId) override;

        void submit(uint32_t imageIndex, size_t const frameId, GraphicsSubmitInfo submission) override;

        GraphicsImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<std::shared_ptr<GraphicsImageView>> getImageViews() const override;

        void resize(vec2ui size);

        /**
         * @brief Returns a buffer containing the data of a recently written to image.
         */
        std::shared_ptr<GraphicsBuffer> getNextReadyBuffer();

    private:
        void createImages();
    };
}