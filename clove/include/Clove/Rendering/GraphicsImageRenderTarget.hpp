#pragma once

#include "Clove/Rendering/RenderTarget.hpp"

#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaImageView.hpp>

#include <queue>

namespace clove{
    class GhaFence;
    class GhaSemaphore;
    class GhaGraphicsQueue;
    class GhaTransferQueue;
    class GhaTransferCommandBuffer;
    class GhaBuffer;
    class GhaFactory;
}

namespace clove {
    /**
     * @brief A RenderTarget backed by a GraphicsImages.
     */
    class GraphicsImageRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        GhaImage::Descriptor imageDescriptor{};

        GhaFactory *factory{ nullptr };

        std::unique_ptr<GhaTransferQueue> transferQueue;
        std::unique_ptr<GhaTransferCommandBuffer> transferCommandBuffer;

        std::unique_ptr<GhaFence> frameInFlight;

        std::unique_ptr<GhaImage> renderTargetImage;
        std::unique_ptr<GhaBuffer> renderTargetBuffer;

        bool requiresResize{ false };

        //FUNCTIONS
    public:
        GraphicsImageRenderTarget() = delete;
        GraphicsImageRenderTarget(GhaImage::Descriptor imageDescriptor, GhaFactory *factory);

        GraphicsImageRenderTarget(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget(GraphicsImageRenderTarget &&other) noexcept;

        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget const &other) = delete;
        GraphicsImageRenderTarget &operator=(GraphicsImageRenderTarget &&other) noexcept;

        ~GraphicsImageRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(GhaSemaphore const *const signalSemaphore) override;

        void present(uint32_t imageIndex, std::vector<GhaSemaphore const *> waitSemaphores) override;

        GhaImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<GhaImage *> getImages() const override;

        void resize(vec2ui size);

        /**
         * @brief Returns a buffer containing the data of a recently written to image.
         * The lifetime of the buffer is tied to this object.
         */
        GhaBuffer *getNextReadyBuffer();

    private:
        void createImages();
    };
}