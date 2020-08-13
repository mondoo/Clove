#pragma once

#include "Bulb/Rendering/ShaderBufferTypes.hpp"
#include "Clove/Graphics/GraphicsFactory.hpp"

#include "Bulb/Rendering/RenderingTypes.hpp"

#include <Clove/Graphics/GraphicsTypes.hpp>

//TEMP
#include "Clove/Graphics/DescriptorSetLayout.hpp"
#include "Clove/Graphics/GraphicsBuffer.hpp"

namespace clv::plt {
    class Window;
}

namespace clv::gfx {
    class RenderTarget;
}

namespace blb::rnd {
    class Sprite;
    class Mesh;
    class Material;
}

namespace blb::rnd {
    class ForwardRenderer3D {
        //TYPES
    public:
        struct ComposedCameraData {
            clv::gfx::Viewport viewport;
            CameraRenderData bufferData;
            //std::shared_ptr<clv::gfx::RenderTarget> target;
        };

        //VARIABLES
    private:
        clv::DelegateHandle windowResizeHandle;
        clv::mth::vec2ui windowSize;
        bool needNewSwapchain = false;

        static constexpr size_t maxFramesInFlight = 2;
        size_t currentFrame                       = 0;

        struct ModelViewProj {
            clv::mth::mat4f model;
            clv::mth::mat4f view;
            clv::mth::mat4f proj;
        };
        //We have an array of these because we basically have a UBO per frame in flight
        //TODO: How would we control this for the client? Would they need to request a UBO each frame?
        //	Mesh, Sprite, Material etc. Would need to be set up specifically with this in mind.
        //	Perhaps they can store the data and sort out the buffers when given to the renderer
        std::vector<std::shared_ptr<clv::gfx::GraphicsBuffer>> uniformBuffers;

        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        std::shared_ptr<clv::gfx::GraphicsQueue> graphicsQueue;
        std::shared_ptr<clv::gfx::PresentQueue> presentQueue;
        std::shared_ptr<clv::gfx::TransferQueue> transferQueue;

        std::shared_ptr<clv::gfx::Swapchain> swapchain;
        std::shared_ptr<clv::gfx::GraphicsImage> depthImage;
        std::shared_ptr<clv::gfx::GraphicsImageView> depthImageView;

        std::shared_ptr<clv::gfx::RenderPass> renderPass;
        std::shared_ptr<clv::gfx::DescriptorSetLayout> descriptorSetLayout;

        std::shared_ptr<clv::gfx::PipelineObject> pipelineObject;

        std::vector<std::shared_ptr<clv::gfx::Framebuffer>> swapChainFrameBuffers;

        std::vector<std::shared_ptr<clv::gfx::GraphicsCommandBuffer>> commandBuffers;

        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> renderFinishedSemaphores;
        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> imageAvailableSemaphores;
        std::array<std::shared_ptr<clv::gfx::Fence>, maxFramesInFlight> inFlightFences;
        std::vector<std::shared_ptr<clv::gfx::Fence>> imagesInFlight;

        std::shared_ptr<clv::gfx::DescriptorPool> descriptorPool;
        std::vector<std::shared_ptr<clv::gfx::DescriptorSet>> descriptorSets;

        //FUNCTIONS
    public:
        //TODO: Ctors
        ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API api);
        ~ForwardRenderer3D();

        void begin();

        void submitPrimitive(const std::shared_ptr<clv::gfx::GraphicsBuffer>& vertexBuffer, const std::shared_ptr<clv::gfx::GraphicsBuffer>& indexBuffer, const clv::mth::mat4f& transform, const Material& material);
        void submitQuad(const clv::mth::mat4f& transform, const Material& material); //Just take image view?

        void submitStaticMesh(const std::shared_ptr<rnd::Mesh>& mesh);
        void submitAnimatedMesh(const std::shared_ptr<rnd::Mesh>& mesh);
        void submitLight(const DirectionalLight& light);
        void submitLight(const PointLight& light);
        void submitCamera(const ComposedCameraData& camera);

        void submitWidget(const std::shared_ptr<Sprite>& widget);
        void submitText(const std::shared_ptr<Sprite>& text);

        void end();

        const std::shared_ptr<clv::gfx::GraphicsFactory>& getGraphicsFactory() const;

    private:
        void onWindowResize(const clv::mth::vec2ui& size);
        void recreateSwapchain();

        void createRenderpass();
        void createDepthBuffer();
        void createPipeline();
        void createSwapchainFrameBuffers();
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();//TEMP: Would probably just allocate sets as we needed, not necessarily when the swap chain resizes
        void recordCommandBuffers();

        void updateUniformBuffer(const size_t imageIndex);
    };
}