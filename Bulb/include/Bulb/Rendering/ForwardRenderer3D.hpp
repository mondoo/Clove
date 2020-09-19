#pragma once

#include "Bulb/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/DescriptorSetLayout.hpp>
#include <Clove/Graphics/GraphicsBuffer.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsTypes.hpp>

namespace clv::plt {
    class Window;
}

namespace clv::gfx {
    class RenderTarget;
}

namespace blb::rnd {
    class Camera;
    class Sprite;
    class Mesh;
    class Material;
}

namespace blb::rnd {
    class ForwardRenderer3D {
        //TYPES
    private:
        struct InFlightImageDescriptorSets {
            std::vector<std::shared_ptr<clv::gfx::DescriptorSet>> primitiveSets;
            std::shared_ptr<clv::gfx::DescriptorSet> viewSet;
            std::shared_ptr<clv::gfx::DescriptorSet> lightingSet;
        };

        //VARIABLES
    private:
        clv::DelegateHandle windowResizeHandle;
        clv::mth::vec2ui windowSize;
        bool needNewSwapchain = false;

        static constexpr size_t maxFramesInFlight{ 2 };
        size_t currentFrame{ 0 };
        uint32_t imageIndex{ 0 };

        struct {
            clv::mth::mat4f view;
            clv::mth::mat4f projection;

            std::vector<std::pair<std::shared_ptr<Mesh>, clv::mth::mat4f>> meshes;

            LightDataArray lights;
            LightCount numLights;
        } currentFrameData;

        std::vector<std::vector<std::shared_ptr<clv::gfx::GraphicsBuffer>>> uniformBuffers;
        std::shared_ptr<clv::gfx::Sampler> sampler;

        std::shared_ptr<clv::gfx::GraphicsDevice> graphicsDevice;
        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        std::shared_ptr<clv::gfx::GraphicsQueue> graphicsQueue;
        std::shared_ptr<clv::gfx::PresentQueue> presentQueue;
        std::shared_ptr<clv::gfx::TransferQueue> transferQueue;

        std::shared_ptr<clv::gfx::Swapchain> swapchain;
        std::shared_ptr<clv::gfx::GraphicsImage> depthImage;
        std::shared_ptr<clv::gfx::GraphicsImageView> depthImageView;

        std::shared_ptr<clv::gfx::RenderPass> renderPass;

        std::shared_ptr<clv::gfx::PipelineObject> pipelineObject;

        std::vector<std::shared_ptr<clv::gfx::Framebuffer>> swapChainFrameBuffers;

        std::vector<std::shared_ptr<clv::gfx::GraphicsCommandBuffer>> commandBuffers;

        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> renderFinishedSemaphores;
        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> imageAvailableSemaphores;
        std::array<std::shared_ptr<clv::gfx::Fence>, maxFramesInFlight> inFlightFences;
        std::vector<std::shared_ptr<clv::gfx::Fence>> imagesInFlight;

        std::vector<std::shared_ptr<clv::gfx::DescriptorSetLayout>> descriptorSetLayouts;
        std::vector<InFlightImageDescriptorSets> descriptorSets;//One for each image.
        std::vector<std::shared_ptr<clv::gfx::DescriptorPool>> descriptorPool;

        //FUNCTIONS
    public:
        //TODO: Ctors
        ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API api);
        ~ForwardRenderer3D();

        void begin();

        /**
         * @brief Submit the active camera the renderer will use.
         */
        void submitCamera(const Camera& camera);

        void submitStaticMesh(std::shared_ptr<Mesh> mesh, clv::mth::mat4f transform);
        void submitAnimatedMesh(std::shared_ptr<Mesh> mesh, clv::mth::mat4f transform);
        void submitLight(const DirectionalLight& light);
        void submitLight(const PointLight& light);

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
        std::vector<std::shared_ptr<clv::gfx::GraphicsBuffer>> createUniformBuffers(const uint32_t bufferCount);
        std::shared_ptr<clv::gfx::DescriptorPool> createDescriptorPool(const std::unordered_map<clv::gfx::DescriptorType, uint32_t>& bindingCount, const uint32_t setCount);
    };
}