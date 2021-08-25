#pragma once

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"
#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"
#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"
#include "Clove/Rendering/ShaderBufferTypes.hpp"

#include <Clove/Delegate/DelegateHandle.hpp>
#include <set>
#include <unordered_map>

namespace clove {
    class RenderTarget;
    class Camera;
    class Sprite;
    class Mesh;
    class Material;
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

    private:
        //Data for an entire frame
        struct FrameData {
            ViewData viewData;
            vec3f viewPosition;

            LightDataArray lights;
            DirectionalShadowTransformArray directionalShadowTransforms;

            LightCount numLights;

            std::array<std::array<mat4f, 6>, MAX_LIGHTS> pointShadowTransforms;

            std::vector<MeshInfo> meshes;

#if 0
            std::vector<std::pair<std::shared_ptr<GhaImageView>, mat4f>> widgets;
            std::vector<std::pair<std::shared_ptr<GhaImageView>, mat4f>> text;
#endif
            void forEachMesh(std::function<void(Mesh const &, size_t const index)> func) {
                for(size_t index = 0; auto const &meshInfo : meshes) {
                    func(*meshInfo.mesh, index++);
                }
            }
        };
        //VARIABLES
    private:
        size_t maxFramesInFlight{};
        size_t currentFrame{ 0 };//The current frame we're operating on

        std::unordered_map<std::string, std::string> shaderIncludes;

        std::unique_ptr<RenderTarget> renderTarget;
        DelegateHandle renderTargetPropertyChangedBeginHandle;
        DelegateHandle renderTargetPropertyChangedEndHandle;

        std::unique_ptr<GhaGraphicsQueue> graphicsQueue{};
        std::unique_ptr<GhaComputeQueue> computeQueue{};
        std::unique_ptr<GhaTransferQueue> transferQueue{};

        std::vector<RgFrameCache> frameCaches{};
        RgGlobalCache globalCache;

#if 0
        //'Square' mesh used to render UI
        std::shared_ptr<Mesh> uiMesh;
#endif

        FrameData currentFrameData;

#if 0
        //Geometry passes. TODO: Use vector?
        std::unordered_map<GeometryPass::Id, std::unique_ptr<GeometryPass>> geometryPasses;
#endif

        //Synchronisation obects
        std::vector<std::unique_ptr<GhaSemaphore>> skinningFinishedSemaphores;
        std::vector<std::unique_ptr<GhaSemaphore>> renderFinishedSemaphores;
        std::vector<std::unique_ptr<GhaSemaphore>> imageAvailableSemaphores;
        std::vector<std::unique_ptr<GhaFence>> framesInFlight;
        std::vector<GhaFence *> imagesInFlight;

        //FUNCTIONS
    public:
        ForwardRenderer3D() = delete;
        ForwardRenderer3D(GhaFactory *graphicsFactory, std::unique_ptr<RenderTarget> renderTarget);

        ForwardRenderer3D(ForwardRenderer3D const &other) = delete;
        ForwardRenderer3D(ForwardRenderer3D &&other) noexcept;

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
    };
}
