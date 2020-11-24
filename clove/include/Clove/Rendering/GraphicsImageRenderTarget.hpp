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
}

namespace garlic::clove {
    /**
     * @brief A RenderTarget backed by a GraphicsImages.
     */
    class GraphicsImageRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        garlic::clove::GraphicsImage::Descriptor imageDescriptor;

        std::shared_ptr<garlic::clove::GraphicsQueue> graphicsQueue;
        std::shared_ptr<garlic::clove::TransferQueue> transferQueue;
        std::shared_ptr<garlic::clove::TransferCommandBuffer> transferCommandBuffer;

        std::shared_ptr<garlic::clove::Semaphore> renderFinishedSemaphore;
        std::shared_ptr<garlic::clove::Fence> frameInFlight;

        std::shared_ptr<garlic::clove::GraphicsImage> renderTargetImage;
        std::shared_ptr<garlic::clove::GraphicsImageView> renderTargetView;
        std::shared_ptr<garlic::clove::GraphicsBuffer> renderTargetBuffer;

        //FUNCTIONS
    public:
        GraphicsImageRenderTarget() = delete;
        GraphicsImageRenderTarget(garlic::clove::GraphicsImage::Descriptor imageDescriptor);

        GraphicsImageRenderTarget(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget(GraphicsImageRenderTarget &&other) noexcept;

        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget &&other) noexcept;

        ~GraphicsImageRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(size_t const frameId) override;

        void submit(uint32_t imageIndex, size_t const frameId, garlic::clove::GraphicsSubmitInfo submission) override;

        garlic::clove::GraphicsImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<std::shared_ptr<garlic::clove::GraphicsImageView>> getImageViews() const override;

        void resize(vec2ui size);

        /**
         * @brief Returns a buffer containing the data of a recently written to image.
         */
        std::shared_ptr<garlic::clove::GraphicsBuffer> getNextReadyBuffer();

    private:
        void createImages();
    };
}