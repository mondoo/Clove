#pragma once

#include "Stem/Rendering/ShaderBufferTypes.hpp"

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

namespace garlic::inline stem {
    class Camera;
    class Sprite;
    class Mesh;
    class Material;
    class RenderTarget;
}

namespace garlic::inline stem {
    class ForwardRenderer3D {
        //TYPES
    public:
        struct StaticMeshInfo {
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
            clv::mth::mat4f transform;
        };
        struct AnimatedMeshInfo {
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
            clv::mth::mat4f transform;
            std::array<clv::mth::mat4f, MAX_JOINTS> matrixPalet;
        };

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

            std::vector<StaticMeshInfo> staticMeshes;
            std::vector<AnimatedMeshInfo> animatedMeshes;

            std::vector<std::pair<std::shared_ptr<clv::gfx::GraphicsImageView>, clv::mth::mat4f>> widgets;
            std::vector<std::pair<std::shared_ptr<clv::gfx::GraphicsImageView>, clv::mth::mat4f>> text;

            void forEachStaticMesh(std::function<void(Mesh const &, size_t const index)> func) {
                for(size_t index = 0; auto const &meshInfo : staticMeshes) {
                    func(*meshInfo.mesh, index++);
                }
            }
            void forEachAnimatedMesh(std::function<void(Mesh const &, size_t const index)> func) {
                for(size_t index = std::size(staticMeshes); auto const &meshInfo : animatedMeshes) {
                    func(*meshInfo.mesh, index++);
                }
            }
        };

        //Objects that hold the state / data of each image (in flight)
        struct ImageData {
            std::shared_ptr<clv::gfx::GraphicsCommandBuffer> commandBuffer;
            std::shared_ptr<clv::gfx::GraphicsCommandBuffer> shadowMapCommandBuffer;
            std::shared_ptr<clv::gfx::GraphicsCommandBuffer> cubeShadowMapCommandBuffer;

            std::shared_ptr<clv::gfx::GraphicsBuffer> frameBuffer;               //Holds data used across all meshes (lighting, camera etc.)
            std::vector<std::unique_ptr<clv::gfx::GraphicsBuffer>> objectBuffers;//Holds the data for each object

            //Descriptor pool for sets that change per frame
            std::shared_ptr<clv::gfx::DescriptorPool> frameDescriptorPool;
            //Descriptor pool for sets that are for a single mesh's material
            std::shared_ptr<clv::gfx::DescriptorPool> meshDescriptorPool;
            //Descriptor pool for sets that are for a ui element.
            std::shared_ptr<clv::gfx::DescriptorPool> uiDescriptorPool;

            std::shared_ptr<clv::gfx::DescriptorSet> viewDescriptorSet;
            std::shared_ptr<clv::gfx::DescriptorSet> lightingDescriptorSet;
            std::shared_ptr<clv::gfx::DescriptorSet> uiDescriptorSet;

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
        static size_t constexpr maxFramesInFlight{ 2 };
        size_t currentFrame{ 0 };//The current frame we're operating

        DelegateHandle renderTargetPropertyChangedHandle;
        std::unique_ptr<RenderTarget> renderTarget;
        std::vector<std::shared_ptr<clv::gfx::Framebuffer>> frameBuffers;

        //'Square' mesh used to render UI
        std::unique_ptr<Mesh> uiMesh;

        clv::gfx::GraphicsDevice* graphicsDevice;
        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        std::shared_ptr<clv::gfx::GraphicsQueue> graphicsQueue;

        std::unordered_map<DescriptorSetSlots, std::shared_ptr<clv::gfx::DescriptorSetLayout>> descriptorSetLayouts;

        //Frame / image data objects
        FrameData currentFrameData;
        std::vector<ImageData> inFlightImageData;

        //Samplers passed along with textures
        std::shared_ptr<clv::gfx::Sampler> textureSampler;
        std::shared_ptr<clv::gfx::Sampler> uiSampler;
        std::shared_ptr<clv::gfx::Sampler> shadowSampler;

        //Objects for the final colour render pass
        std::shared_ptr<clv::gfx::RenderPass> renderPass;
        std::shared_ptr<clv::gfx::PipelineObject> staticMeshPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> animatedMeshPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> widgetPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> textPipelineObject;

        std::shared_ptr<clv::gfx::GraphicsImage> depthImage;
        std::shared_ptr<clv::gfx::GraphicsImageView> depthImageView;

        //Objects for the shadow map pass
        std::shared_ptr<clv::gfx::RenderPass> shadowMapRenderPass;
        std::shared_ptr<clv::gfx::PipelineObject> staticMeshShadowMapPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> animatedMeshShadowMapPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> staticMeshCubeShadowMapPipelineObject;
        std::shared_ptr<clv::gfx::PipelineObject> animatedMeshCubeShadowMapPipelineObject;

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
        ForwardRenderer3D(std::unique_ptr<RenderTarget> renderTarget);

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

        void submitStaticMesh(StaticMeshInfo meshInfo);
        void submitAnimatedMesh(AnimatedMeshInfo meshInfo);
        void submitLight(DirectionalLight const &light);
        void submitLight(PointLight const &light);

        void submitWidget(std::shared_ptr<clv::gfx::GraphicsImageView> const widget, clv::mth::mat4f const modelProjection);
        void submitText(std::shared_ptr<clv::gfx::GraphicsImageView> const text, clv::mth::mat4f const modelProjection);

        void end();

    private:
        void recreateSwapchain();

        void createRenderpass();
        void createShadowMapRenderpass();

        void createDepthBuffer();

        void createPipeline();
        void createShadowMapPipeline();
        void createCubeShadowMapPipeline();
        void createUiPipeline();

        void createRenderTargetFrameBuffers();

        std::shared_ptr<clv::gfx::DescriptorPool> createDescriptorPool(std::unordered_map<clv::gfx::DescriptorType, uint32_t> const &bindingCount, uint32_t const setCount);
    };
}