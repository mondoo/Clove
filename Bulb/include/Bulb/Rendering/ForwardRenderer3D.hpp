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

            clv::mth::vec3f viewPosition;

            std::vector<std::pair<std::shared_ptr<Mesh>, clv::mth::mat4f>> meshes;

            LightDataArray lights;
            DirectionalShadowTransformArray directionalShadowTransforms;

            LightCount numLights;
        } currentFrameData;

        //Contains size / offsets for each piece of data placed inside the per frame uniform buffer
        struct {
            size_t viewSize{ 0 };
            size_t viewPosSize{ 0 };
            size_t lightSize{ 0 };
            size_t numLightsSize{ 0 };
            size_t shadowTransformSize{ 0 };

            size_t viewOffset{ 0 };
            size_t viewPosOffset{ 0 };
            size_t lightOffset{ 0 };
            size_t numLightsOffset{ 0 };
            size_t shadowTransformOffset{ 0 };

            size_t totalSize() const {
                return shadowTransformOffset + shadowTransformSize;
            }
        } uniformBufferLayout;

        std::shared_ptr<clv::gfx::GraphicsDevice> graphicsDevice;
        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        std::shared_ptr<clv::gfx::GraphicsQueue> graphicsQueue;
        std::shared_ptr<clv::gfx::PresentQueue> presentQueue;
        std::shared_ptr<clv::gfx::TransferQueue> transferQueue;

        std::shared_ptr<clv::gfx::Swapchain> swapchain;
        std::shared_ptr<clv::gfx::RenderPass> renderPass;
        std::shared_ptr<clv::gfx::PipelineObject> pipelineObject;

        std::vector<std::shared_ptr<clv::gfx::Framebuffer>> swapChainFrameBuffers;

        std::vector<std::shared_ptr<clv::gfx::GraphicsCommandBuffer>> commandBuffers;

        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> renderFinishedSemaphores;
        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> imageAvailableSemaphores;
        std::array<std::shared_ptr<clv::gfx::Fence>, maxFramesInFlight> inFlightFences;
        std::vector<std::shared_ptr<clv::gfx::Fence>> imagesInFlight;

        std::vector<std::shared_ptr<clv::gfx::DescriptorSetLayout>> descriptorSetLayouts;
        std::vector<InFlightImageDescriptorSets> descriptorSets;                      //One for each image.
        std::vector<std::shared_ptr<clv::gfx::DescriptorPool>> materialDescriptorPool;//One for each image.
        std::vector<std::shared_ptr<clv::gfx::DescriptorPool>> frameDescriptorPool;   //One for each image. For descriptors which are set once per frame

        std::vector<std::shared_ptr<clv::gfx::GraphicsBuffer>> uniformBuffers;//One per image. Currently no per mesh data is stored in a buffer
        std::shared_ptr<clv::gfx::Sampler> sampler;                           //Generic sampler passed along with textures

        std::shared_ptr<clv::gfx::GraphicsImage> depthImage;
        std::shared_ptr<clv::gfx::GraphicsImageView> depthImageView;

        std::vector<std::shared_ptr<clv::gfx::GraphicsImage>> shadowMaps;
        std::vector<std::shared_ptr<clv::gfx::GraphicsImageView>> shadowMapViews;
        std::vector<std::shared_ptr<clv::gfx::Framebuffer>> shadowMapFrameBuffers;
        std::shared_ptr<clv::gfx::RenderPass> shadowMapRenderPass;
        std::shared_ptr<clv::gfx::PipelineObject> shadowMapPipelineObject;

        //FUNCTIONS
    public:
        ForwardRenderer3D() = delete;
        ForwardRenderer3D(clv::plt::Window& window, const clv::gfx::API api);

        ForwardRenderer3D(const ForwardRenderer3D& other) = delete;
        ForwardRenderer3D(ForwardRenderer3D&& other) noexcept;

        ForwardRenderer3D& operator=(const ForwardRenderer3D& other) = delete;
        ForwardRenderer3D& operator=(ForwardRenderer3D&& other) noexcept;

        ~ForwardRenderer3D();

        void begin();

        /**
         * @brief Submit the active camera the renderer will use.
         */
        void submitCamera(const Camera& camera, clv::mth::vec3f position);

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