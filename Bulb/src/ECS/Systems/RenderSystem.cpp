#include "Bulb/ECS/Systems/RenderSystem.hpp"

#include "Bulb/ECS/Components/AnimatedModelComponent.hpp"
#include "Bulb/ECS/Components/CameraComponent.hpp"
#include "Bulb/ECS/Components/DirectionalLightComponent.hpp"
#include "Bulb/ECS/Components/PointLightComponent.hpp"
#include "Bulb/ECS/Components/StaticModelComponent.hpp"
#include "Bulb/ECS/Components/TransformComponent.hpp"
#include "Bulb/ECS/World.hpp"
#include "Bulb/Rendering/ForwardRenderer3D.hpp"
#include "Bulb/Rendering/Renderables/Mesh.hpp"

using namespace clv;
using namespace clv::gfx;
using namespace blb::rnd;

namespace blb::ecs {
    RenderSystem::RenderSystem(std::shared_ptr<ForwardRenderer3D> renderer)
        : renderer(std::move(renderer)) {
    }

    RenderSystem::RenderSystem(RenderSystem &&other) noexcept = default;

    RenderSystem &RenderSystem::operator=(RenderSystem &&other) noexcept = default;

    RenderSystem::~RenderSystem() = default;

    void RenderSystem::update(World &world, utl::DeltaTime deltaTime) {
        CLV_PROFILE_FUNCTION();

        //Transform and submit cameras
        for(auto &&[transform, camera] : world.getComponentSets<TransformComponent, CameraComponent>()) {
            mth::vec3f const position = transform->getPosition(TransformSpace::World);

            mth::vec3f const camFront = transform->getForward();
            mth::vec3f const camUp    = transform->getUp();

            mth::mat4f const lookAt = mth::lookAt(position, position + camFront, camUp);
            camera->setView(lookAt);

            renderer->submitCamera(camera->camera, std::move(position));
        }

        //Submit static meshes
        for(auto &&[transform, staticModel] : world.getComponentSets<TransformComponent, StaticModelComponent>()) {
            mth::mat4f const modelTransform = transform->getTransformationMatrix(TransformSpace::World);

            for(auto &mesh : staticModel->model.getMeshes()) {
                renderer->submitStaticMesh(mesh, modelTransform);
            }
        }
        //Submit animated meshes
        for(auto &&[transform, animatedModel] : world.getComponentSets<TransformComponent, AnimedModelComponent>()) {
            mth::mat4f const modelTransform = transform->getTransformationMatrix(TransformSpace::World);
            auto const matrixPalet          = animatedModel->model.update(deltaTime);

            for(auto &mesh : animatedModel->model.getMeshes()) {
                renderer->submitAnimatedMesh(mesh, modelTransform, matrixPalet);
            }
        }

        //Submit directional lights
        for(auto &&[light] : world.getComponentSets<DirectionalLightComponent>()) {
            light->lightData.shadowTransform = light->shadowProj * mth::lookAt(-mth::normalise(light->lightData.data.direction) * (light->farDist / 2.0f), mth::vec3f{ 0.0f, 0.0f, 0.0f }, mth::vec3f{ 0.0f, 1.0f, 0.0f });

            renderer->submitLight(light->lightData);
        }

        //Submit point lights
        for(auto &&[transform, light] : world.getComponentSets<TransformComponent, PointLightComponent>()) {
            mth::vec3f const &position = transform->getPosition();

            light->lightData.data.position       = position;
            light->lightData.shadowTransforms[0] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 1.0f, 0.0f, 0.0f }, mth::vec3f{ 0.0f, 1.0f, 0.0f });
            light->lightData.shadowTransforms[1] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ -1.0f, 0.0f, 0.0f }, mth::vec3f{ 0.0f, 1.0f, 0.0f });
            light->lightData.shadowTransforms[2] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0f, 1.0f, 0.0f }, mth::vec3f{ 0.0f, 0.0f, -1.0f });
            light->lightData.shadowTransforms[3] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0f, -1.0f, 0.0f }, mth::vec3f{ 0.0f, 0.0f, 1.0f });
            light->lightData.shadowTransforms[4] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0f, 0.0f, 1.0f }, mth::vec3f{ 0.0f, 1.0f, 0.0f });
            light->lightData.shadowTransforms[5] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0f, 0.0f, -1.0f }, mth::vec3f{ 0.0f, 1.0f, 0.0f });

            renderer->submitLight(light->lightData);
        }
    }
}