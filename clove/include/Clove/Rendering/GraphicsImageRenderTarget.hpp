#pragma once

#include "Clove/Rendering/RenderTarget.hpp"

#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaImageView.hpp>

#include <queue>

namespace garlic::clove{
    class GhaFence;
    class GhaSemaphore;
    class GhaGraphicsQueue;
    class GhaTransferQueue;
    class GhaTransferCommandBuffer;
    class GhaBuffer;
    class GhaFactory;
}

namespace garlic::clove {
    /**
     * @brief A RenderTarget backed by a GraphicsImages.
     */
    class GraphicsImageRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        GhaImage::Descriptor imageDescriptor{};

        std::shared_ptr<GhaFactory> factory;

        std::shared_ptr<GhaTransferQueue> transferQueue;
        std::shared_ptr<GhaTransferCommandBuffer> transferCommandBuffer;

        std::shared_ptr<GhaFence> frameInFlight;

        std::shared_ptr<GhaImage> renderTargetImage;
        std::shared_ptr<GhaImageView> renderTargetView;
        std::shared_ptr<GhaBuffer> renderTargetBuffer;

        //FUNCTIONS
    public:
        GraphicsImageRenderTarget() = delete;
        GraphicsImageRenderTarget(GhaImage::Descriptor imageDescriptor, std::shared_ptr<GhaFactory> factory);

        GraphicsImageRenderTarget(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget(GraphicsImageRenderTarget &&other) noexcept;

        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget &&other) noexcept;

        ~GraphicsImageRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(std::shared_ptr<GhaSemaphore> signalSemaphore) override;

        void present(uint32_t imageIndex, std::vector<std::shared_ptr<GhaSemaphore>> waitSemaphores) override;

        GhaImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<std::shared_ptr<GhaImageView>> getImageViews() const override;

        void resize(vec2ui size);

        /**
         * @brief Returns a buffer containing the data of a recently written to image.
         */
        std::shared_ptr<GhaBuffer> getNextReadyBuffer();

    private:
        void createImages();
    };
}