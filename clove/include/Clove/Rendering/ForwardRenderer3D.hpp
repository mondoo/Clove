#pragma once

#include "Clove/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Graphics/DescriptorSetLayout.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <Clove/Graphics/GraphicsBuffer.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Delegate/DelegateHandle.hpp>

namespace garlic::clove {
    class RenderTarget;
    class Window;
    class Camera;
    class Sprite;
    class Mesh;
    class Material;
    class RenderTarget;
}

namespace garlic::clove {
    class ForwardRenderer3D {
        //TYPES
    public:
        struct StaticMeshInfo {
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
            mat4f transform;
        };
        struct AnimatedMeshInfo {
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
            mat4f transform;
            std::array<mat4f, MAX_JOINTS> matrixPalet;
        };

    private:
        //Data for an entire frame
        struct FrameData {
            //Frame data that directly translates into a UBO
            struct BufferData {
                //TODO: Get the alignment from vulkan
                alignas(256) ViewData viewData;
                alignas(256) vec3f viewPosition;

                alignas(256) LightDataArray lights;
                alignas(256) DirectionalShadowTransformArray directionalShadowTransforms;

                alignas(256) LightCount numLights;
            } bufferData;

            std::array<std::array<mat4f, 6>, MAX_LIGHTS> pointShadowTransforms;

            std::vector<StaticMeshInfo> staticMeshes;
            std::vector<AnimatedMeshInfo> animatedMeshes;

            std::vector<std::pair<std::shared_ptr<garlic::clove::GraphicsImageView>, mat4f>> widgets;
            std::vector<std::pair<std::shared_ptr<garlic::clove::GraphicsImageView>, mat4f>> text;

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
            std::shared_ptr<garlic::clove::GraphicsCommandBuffer> commandBuffer;
            std::shared_ptr<garlic::clove::GraphicsCommandBuffer> shadowMapCommandBuffer;
            std::shared_ptr<garlic::clove::GraphicsCommandBuffer> cubeShadowMapCommandBuffer;

            std::shared_ptr<garlic::clove::GraphicsBuffer> frameDataBuffer;               //Holds data used across all meshes (lighting, camera etc.)
            std::vector<std::unique_ptr<garlic::clove::GraphicsBuffer>> objectBuffers;//Holds the data for each object

            //Descriptor pool for sets that change per frame
            std::shared_ptr<garlic::clove::DescriptorPool> frameDescriptorPool;
            //Descriptor pool for sets that are for a single mesh's material
            std::shared_ptr<garlic::clove::DescriptorPool> meshDescriptorPool;
            //Descriptor pool for sets that are for a ui element.
            std::shared_ptr<garlic::clove::DescriptorPool> uiDescriptorPool;

            std::shared_ptr<garlic::clove::DescriptorSet> viewDescriptorSet;
            std::shared_ptr<garlic::clove::DescriptorSet> lightingDescriptorSet;
            std::shared_ptr<garlic::clove::DescriptorSet> uiDescriptorSet;

            std::array<std::shared_ptr<garlic::clove::GraphicsImage>, MAX_LIGHTS> shadowMaps;
            std::array<std::shared_ptr<garlic::clove::GraphicsImageView>, MAX_LIGHTS> shadowMapViews;
            std::array<std::shared_ptr<garlic::clove::Framebuffer>, MAX_LIGHTS> shadowMapFrameBuffers;

            std::array<std::shared_ptr<garlic::clove::GraphicsImage>, MAX_LIGHTS> cubeShadowMaps;
            std::array<std::shared_ptr<garlic::clove::GraphicsImageView>, MAX_LIGHTS> cubeShadowMapViews;                   //Views the whole cube
            std::array<std::array<std::shared_ptr<garlic::clove::GraphicsImageView>, 6>, MAX_LIGHTS> cubeShadowMapFaceViews;//Views each side of the cube. For the frame buffer
            std::array<std::array<std::shared_ptr<garlic::clove::Framebuffer>, 6>, MAX_LIGHTS> cubeShadowMapFrameBuffers;
        };

        //VARIABLES
    private:
        static size_t constexpr maxFramesInFlight{ 2 };
        size_t currentFrame{ 0 };//The current frame we're operating on

        DelegateHandle renderTargetPropertyChangedHandle;
        std::unique_ptr<RenderTarget> renderTarget;
        std::vector<std::shared_ptr<garlic::clove::Framebuffer>> frameBuffers;

        //'Square' mesh used to render UI
        std::unique_ptr<Mesh> uiMesh;

        garlic::clove::GraphicsDevice* graphicsDevice;
        std::shared_ptr<garlic::clove::GraphicsFactory> graphicsFactory;

        std::shared_ptr<garlic::clove::GraphicsQueue> graphicsQueue;

        std::unordered_map<DescriptorSetSlots, std::shared_ptr<garlic::clove::DescriptorSetLayout>> descriptorSetLayouts;

        //Frame / image data objects
        FrameData currentFrameData;
        std::vector<ImageData> inFlightImageData;

        //Samplers passed along with textures
        std::shared_ptr<garlic::clove::Sampler> textureSampler;
        std::shared_ptr<garlic::clove::Sampler> uiSampler;
        std::shared_ptr<garlic::clove::Sampler> shadowSampler;

        //Objects for the final colour render pass
        std::shared_ptr<garlic::clove::RenderPass> renderPass;
        std::shared_ptr<garlic::clove::PipelineObject> staticMeshPipelineObject;
        std::shared_ptr<garlic::clove::PipelineObject> animatedMeshPipelineObject;
        std::shared_ptr<garlic::clove::PipelineObject> widgetPipelineObject;
        std::shared_ptr<garlic::clove::PipelineObject> textPipelineObject;

        std::shared_ptr<garlic::clove::GraphicsImage> depthImage;
        std::shared_ptr<garlic::clove::GraphicsImageView> depthImageView;

        //Objects for the shadow map pass
        std::shared_ptr<garlic::clove::RenderPass> shadowMapRenderPass;
        std::shared_ptr<garlic::clove::PipelineObject> staticMeshShadowMapPipelineObject;
        std::shared_ptr<garlic::clove::PipelineObject> animatedMeshShadowMapPipelineObject;
        std::shared_ptr<garlic::clove::PipelineObject> staticMeshCubeShadowMapPipelineObject;
        std::shared_ptr<garlic::clove::PipelineObject> animatedMeshCubeShadowMapPipelineObject;

        //Synchronisation obects
        std::array<std::shared_ptr<garlic::clove::Semaphore>, maxFramesInFlight> shadowFinishedSemaphores;
        std::array<std::shared_ptr<garlic::clove::Semaphore>, maxFramesInFlight> cubeShadowFinishedSemaphores;

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
        void submitCamera(Camera const &camera, vec3f position);

        void submitStaticMesh(StaticMeshInfo meshInfo);
        void submitAnimatedMesh(AnimatedMeshInfo meshInfo);
        void submitLight(DirectionalLight const &light);
        void submitLight(PointLight const &light);

        void submitWidget(std::shared_ptr<garlic::clove::GraphicsImageView> const widget, mat4f const modelProjection);
        void submitText(std::shared_ptr<garlic::clove::GraphicsImageView> const text, mat4f const modelProjection);

        void end();

    private:
        void onRenderTargetPropertiesChanged();

        void createRenderpass();
        void createShadowMapRenderpass();

        void createDepthBuffer();

        void createPipeline();
        void createShadowMapPipeline();
        void createCubeShadowMapPipeline();
        void createUiPipeline();

        void createRenderTargetFrameBuffers();

        std::shared_ptr<garlic::clove::DescriptorPool> createDescriptorPool(std::unordered_map<garlic::clove::DescriptorType, uint32_t> const &bindingCount, uint32_t const setCount);
    };
}