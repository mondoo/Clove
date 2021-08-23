#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"
#include "Clove/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Delegate/DelegateHandle.hpp>
#include <Clove/Graphics/GhaBuffer.hpp>
#include <Clove/Graphics/GhaDescriptorSetLayout.hpp>
#include <Clove/Graphics/GhaDevice.hpp>
#include <Clove/Graphics/GhaFactory.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <set>
#include <unordered_map>

namespace clove {
    class RenderTarget;
    class Window;
    class Camera;
    class Sprite;
    class Mesh;
    class Material;
    class RenderTarget;
    class GhaComputeQueue;
}

namespace clove {
    class ForwardRenderer3D {
        //TYPES
    public:
        //TODO: Currently transform and matrixPalet are copied per mesh for each model. This should be avoided
        struct MeshInfo {
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
            mat4f transform;
            std::array<mat4f, MAX_JOINTS> matrixPalet;
        };

        enum class DescriptorSetSlots {
            Mesh,
            View,
            Lighting,
            UI,
        };

    private:
        //Data for an entire frame
        struct FrameData {
            //Frame data that directly translates into a UBO
            struct BufferData {
                //TODO: Get the alignment from vulkan
                alignas(256) ViewData viewData; //NOLINT
                alignas(256) vec3f viewPosition;//NOLINT

                alignas(256) LightDataArray lights;                                      //NOLINT
                alignas(256) DirectionalShadowTransformArray directionalShadowTransforms;//NOLINT

                alignas(256) LightCount numLights;//NOLINT
            } bufferData;

            std::array<std::array<mat4f, 6>, MAX_LIGHTS> pointShadowTransforms;

            std::vector<MeshInfo> meshes;

            std::vector<std::pair<std::shared_ptr<GhaImageView>, mat4f>> widgets;
            std::vector<std::pair<std::shared_ptr<GhaImageView>, mat4f>> text;

            void forEachMesh(std::function<void(Mesh const &, size_t const index)> func) {
                for(size_t index = 0; auto const &meshInfo : meshes) {
                    func(*meshInfo.mesh, index++);
                }
            }
        };

        /**
         * @brief Objects that hold the state / data of each image (in flight)
         */
        struct ImageData {
            std::unique_ptr<GhaGraphicsCommandBuffer> commandBuffer;
            std::unique_ptr<GhaGraphicsCommandBuffer> shadowMapCommandBuffer;
            std::unique_ptr<GhaGraphicsCommandBuffer> cubeShadowMapCommandBuffer;
            std::unique_ptr<GhaComputeCommandBuffer> skinningCommandBuffer;

            std::unique_ptr<GhaBuffer> frameDataBuffer;            /**< Holds data used across all meshes (lighting, camera etc.). */
            std::vector<std::unique_ptr<GhaBuffer>> objectBuffers; /**< Holds the data for each object. */

            std::unique_ptr<GhaDescriptorPool> frameDescriptorPool; /**< Descriptor pool for sets that change per frame. */
            std::unique_ptr<GhaDescriptorPool> meshDescriptorPool;  /**< Descriptor pool for sets that are for a single mesh's material. */
            std::unique_ptr<GhaDescriptorPool> uiDescriptorPool;    /**< Descriptor pool for sets that are for a ui element. */
            std::unique_ptr<GhaDescriptorPool> skinningDescriptorPool;

            std::unique_ptr<GhaDescriptorSet> viewDescriptorSet;
            std::unique_ptr<GhaDescriptorSet> lightingDescriptorSet;
            std::unique_ptr<GhaDescriptorSet> uiDescriptorSet;

            std::unique_ptr<GhaImage> shadowMaps;
            std::unique_ptr<GhaImageView> shadowMapViews;//View over entire array. For sampling in lighting shader.
            std::array<std::unique_ptr<GhaImageView>, MAX_LIGHTS> shadowMapArrayLayerViews;//Views for each element of the array. For the frame buffer
            std::array<std::unique_ptr<GhaFramebuffer>, MAX_LIGHTS> shadowMapFrameBuffers;

            std::unique_ptr<GhaImage> cubeShadowMaps;
            std::unique_ptr<GhaImageView> cubeShadowMapViews;                                                           //Views the whole cube. For sampling in lighting shader.
            std::array<std::array<std::unique_ptr<GhaImageView>, cubeMapLayerCount>, MAX_LIGHTS> cubeShadowMapFaceViews;//Views each side of the cube. For the frame buffer
            std::array<std::array<std::unique_ptr<GhaFramebuffer>, cubeMapLayerCount>, MAX_LIGHTS> cubeShadowMapFrameBuffers;
        };

        //VARIABLES
    private:
        size_t maxFramesInFlight{};
        size_t currentFrame{ 0 };//The current frame we're operating on

        std::unordered_map<std::string, std::string> shaderIncludes;

        DelegateHandle renderTargetPropertyChangedBeginHandle;
        DelegateHandle renderTargetPropertyChangedEndHandle;
        std::unique_ptr<RenderTarget> renderTarget;
        std::vector<std::unique_ptr<GhaFramebuffer>> frameBuffers;//TODO: Move inside the ImageData

        //'Square' mesh used to render UI
        std::shared_ptr<Mesh> uiMesh;

        GhaDevice *ghaDevice;
        GhaFactory *ghaFactory;

        std::unique_ptr<GhaGraphicsQueue> graphicsQueue;
        std::unique_ptr<GhaComputeQueue> computeQueue;

        std::unordered_map<DescriptorSetSlots, std::unique_ptr<GhaDescriptorSetLayout>> descriptorSetLayouts;
        std::unique_ptr<GhaDescriptorSetLayout> skinningSetLayout;

        //Frame / image data objects
        FrameData currentFrameData;
        std::vector<ImageData> inFlightImageData;

        //Samplers passed along with textures
        std::unique_ptr<GhaSampler> textureSampler;
        std::unique_ptr<GhaSampler> uiSampler;
        std::unique_ptr<GhaSampler> shadowSampler;

        //Geometry passes. TODO: Use vector?
        std::unordered_map<GeometryPass::Id, std::unique_ptr<GeometryPass>> geometryPasses;

        //Objects for the final colour render pass
        std::unique_ptr<GhaRenderPass> renderPass;
        std::unique_ptr<GhaGraphicsPipelineObject> widgetPipelineObject;
        std::unique_ptr<GhaGraphicsPipelineObject> textPipelineObject;

        std::unique_ptr<GhaImage> depthImage;
        std::unique_ptr<GhaImageView> depthImageView;

        //Objects for the shadow map pass
        std::unique_ptr<GhaRenderPass> shadowMapRenderPass;

        //Synchronisation obects
        std::vector<std::unique_ptr<GhaSemaphore>> skinningFinishedSemaphores;
        std::vector<std::unique_ptr<GhaSemaphore>> renderFinishedSemaphores;
        std::vector<std::unique_ptr<GhaSemaphore>> imageAvailableSemaphores;
        std::vector<std::unique_ptr<GhaFence>> framesInFlight;
        std::vector<GhaFence *> imagesInFlight;

        //TEMP: Compute skinning objects -- Put inside a GeometryPass
        std::unique_ptr<GhaComputePipelineObject> skinningPipeline;

        //FUNCTIONS
    public:
        ForwardRenderer3D() = delete;
        ForwardRenderer3D(GhaDevice *graphicsDevice, std::unique_ptr<RenderTarget> renderTarget);

        ForwardRenderer3D(ForwardRenderer3D const &other) = delete;
        //ForwardRenderer3D(ForwardRenderer3D&& other) noexcept;

        ForwardRenderer3D &operator=(ForwardRenderer3D const &other) = delete;
        ForwardRenderer3D &operator=(ForwardRenderer3D &&other) noexcept;

        ~ForwardRenderer3D();

        void begin();

        void submitMesh(MeshInfo meshInfo, std::set<GeometryPass::Id> const &geometryPassIds);

        /**
         * @brief Submit the active camera the renderer will use.
         */
        void submitCamera(Camera const &camera, vec3f position);

        void submitLight(DirectionalLight const &light);
        void submitLight(PointLight const &light);

        void submitWidget(std::shared_ptr<GhaImageView> const &widget, mat4f const modelProjection);
        void submitText(std::shared_ptr<GhaImageView> const &text, mat4f const modelProjection);

        void end();

    private:
        void cleanupRenderTargetResources();
        void createRenderTargetResources();

        void createRenderpass();
        void createShadowMapRenderpass();

        void createDepthBuffer();

        void createUiPipeline();

        void createRenderTargetFrameBuffers();

        std::unique_ptr<GhaDescriptorPool> createDescriptorPool(std::unordered_map<DescriptorType, uint32_t> const &bindingCount, uint32_t const setCount);
    };
}
