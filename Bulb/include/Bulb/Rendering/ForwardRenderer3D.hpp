#pragma once

#include "Bulb/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/DescriptorSetLayout.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <Clove/Graphics/GraphicsBuffer.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Root/Delegate/DelegateHandle.hpp>

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
        //Data for an entire frame
        struct FrameData {
            //Frame data that directly translates into a UBO
            struct BufferData {
                //TODO: Get the alignment from vulkan
                alignas(256) ViewData viewData;
                alignas(256) clv::mth::vec3f viewPosition;

                alignas(256) LightDataArray lights;
                alignas(256) DirectionalShadowTransformArray directionalShadowTransforms;

                alignas(256) LightCount numLights;
            } bufferData;

            std::array<std::array<clv::mth::mat4f, 6>, MAX_LIGHTS> pointShadowTransforms;

            std::vector<std::pair<std::shared_ptr<Mesh>, clv::mth::mat4f>> meshes;
        };

        //Objects that hold the state / data of each image (in flight)
        struct ImageData {
            std::shared_ptr<clv::gfx::GraphicsCommandBuffer> commandBuffer;
            std::shared_ptr<clv::gfx::GraphicsCommandBuffer> shadowMapCommandBuffer;
            std::shared_ptr<clv::gfx::GraphicsCommandBuffer> cubeShadowMapCommandBuffer;

            std::shared_ptr<clv::gfx::GraphicsBuffer> uniformBuffer;

            //Descriptor pool for sets that change per frame
            std::shared_ptr<clv::gfx::DescriptorPool> frameDescriptorPool;
            //Descriptor pool for sets that are for a single mesh's material
            std::shared_ptr<clv::gfx::DescriptorPool> materialDescriptorPool;

            std::shared_ptr<clv::gfx::DescriptorSet> viewDescriptorSet;
            std::shared_ptr<clv::gfx::DescriptorSet> lightingDescriptorSet;

            std::array<std::shared_ptr<clv::gfx::GraphicsImage>, MAX_LIGHTS> shadowMaps;
            std::array<std::shared_ptr<clv::gfx::GraphicsImageView>, MAX_LIGHTS> shadowMapViews;
            std::array<std::shared_ptr<clv::gfx::Framebuffer>, MAX_LIGHTS> shadowMapFrameBuffers;

            std::array<std::shared_ptr<clv::gfx::GraphicsImage>, MAX_LIGHTS> cubeShadowMaps;
            std::array<std::shared_ptr<clv::gfx::GraphicsImageView>, MAX_LIGHTS> cubeShadowMapViews;                   //Views the whole cube
            std::array<std::array<std::shared_ptr<clv::gfx::GraphicsImageView>, 6>, MAX_LIGHTS> cubeShadowMapFaceViews;//Views each side of the cube. For the frame buffer
            std::array<std::array<std::shared_ptr<clv::gfx::Framebuffer>, 6>, MAX_LIGHTS> cubeShadowMapFrameBuffers;
        };

        //VARIABLES
    private:
        garlic::DelegateHandle windowResizeHandle;
        clv::mth::vec2ui windowSize;
        bool needNewSwapchain{ false };

        static size_t constexpr maxFramesInFlight{ 2 };
        size_t currentFrame{ 0 };//The current frame we're operating on

        std::shared_ptr<clv::gfx::GraphicsDevice> graphicsDevice;
        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        std::shared_ptr<clv::gfx::GraphicsQueue> graphicsQueue;
        std::shared_ptr<clv::gfx::PresentQueue> presentQueue;

        std::shared_ptr<clv::gfx::Swapchain> swapchain;
        std::vector<std::shared_ptr<clv::gfx::Framebuffer>> swapChainFrameBuffers;

        std::unordered_map<DescriptorSetSlots, std::shared_ptr<clv::gfx::DescriptorSetLayout>> descriptorSetLayouts;

        //Frame / image data objects
        FrameData currentFrameData;
        std::vector<ImageData> inFlightImageData;

        //Generic sampler passed along with textures
        std::shared_ptr<clv::gfx::Sampler> sampler;

        //Objects for the final colour render pass
        std::shared_ptr<clv::gfx::RenderPass> renderPass;
        std::shared_ptr<clv::gfx::PipelineObject> pipelineObject;

        std::shared_ptr<clv::gfx::GraphicsImage> depthImage;
        std::shared_ptr<clv::gfx::GraphicsImageView> depthImageView;

        //Objects for the shadow map pass
        std::shared_ptr<clv::gfx::RenderPass> shadowMapRenderPass;
        std::shared_ptr<clv::gfx::PipelineObject> shadowMapPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> cubeShadowMapPipelineObject;

        //Synchronisation obects
        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> shadowFinishedSemaphores;
        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> cubeShadowFinishedSemaphores;
        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> renderFinishedSemaphores;
        std::array<std::shared_ptr<clv::gfx::Semaphore>, maxFramesInFlight> imageAvailableSemaphores;
        std::array<std::shared_ptr<clv::gfx::Fence>, maxFramesInFlight> inFlightFences;
        std::vector<std::shared_ptr<clv::gfx::Fence>> imagesInFlight;

        //FUNCTIONS
    public:
        ForwardRenderer3D() = delete;
        ForwardRenderer3D(clv::plt::Window &window, clv::gfx::API const api);

        ForwardRenderer3D(ForwardRenderer3D const &other) = delete;
        //ForwardRenderer3D(ForwardRenderer3D&& other) noexcept;

        ForwardRenderer3D &operator=(ForwardRenderer3D const &other) = delete;
        ForwardRenderer3D &operator=(ForwardRenderer3D &&other) noexcept;

        ~ForwardRenderer3D();

        void begin();

        /**
         * @brief Submit the active camera the renderer will use.
         */
        void submitCamera(Camera const &camera, clv::mth::vec3f position);

        void submitStaticMesh(std::shared_ptr<Mesh> mesh, clv::mth::mat4f transform);
        void submitAnimatedMesh(std::shared_ptr<Mesh> mesh, clv::mth::mat4f transform);
        void submitLight(DirectionalLight const &light);
        void submitLight(PointLight const &light);

        void submitWidget(std::shared_ptr<Sprite> const &widget);
        void submitText(std::shared_ptr<Sprite> const &text);

        void end();

        std::shared_ptr<clv::gfx::GraphicsFactory> const &getGraphicsFactory() const;

    private:
        void onWindowResize(clv::mth::vec2ui const &size);
        void recreateSwapchain();

        void createRenderpass();
        void createShadowMapRenderpass();
        //void createCubeShadowMapRenderpass();

        void createDepthBuffer();

        void createPipeline();
        void createShadowMapPipeline();
        void createCubeShadowMapPipeline();

        void createSwapchainFrameBuffers();

        std::shared_ptr<clv::gfx::GraphicsBuffer> createUniformBuffer();
        std::shared_ptr<clv::gfx::DescriptorPool> createDescriptorPool(std::unordered_map<clv::gfx::DescriptorType, uint32_t> const &bindingCount, uint32_t const setCount);
    };
}