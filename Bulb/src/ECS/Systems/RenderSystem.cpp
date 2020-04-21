#include "Bulb/ECS/Systems/RenderSystem.hpp"

#include "Bulb/ECS/Components/CameraComponent.hpp"
#include "Bulb/ECS/Components/DirectionalLightComponent.hpp"
#include "Bulb/ECS/Components/ModelComponent.hpp"
#include "Bulb/ECS/Components/PointLightComponent.hpp"
#include "Bulb/ECS/Components/TransformComponent.hpp"
#include "Bulb/ECS/World.hpp"
#include "Bulb/Rendering/Renderables/Mesh.hpp"
#include "Bulb/Rendering/Renderer.hpp"

#include <Clove/Graphics/CommandBuffer.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/PipelineObject.hpp>
#include <Clove/Graphics/RenderTarget.hpp>
#include <Clove/Graphics/Texture.hpp>
#include <Clove/Platform/Window.hpp>

using namespace clv;
using namespace clv::gfx;
using namespace blb::rnd;

namespace blb::ecs {
	RenderSystem::RenderSystem(std::unique_ptr<Renderer> renderer)
		: renderer(std::move(renderer)) {
	}

	RenderSystem::RenderSystem(plt::Window& window) {
		renderer = std::make_unique<Renderer>(window);
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::preUpdate(World& world) {
		for(auto [camera] : world.getComponentSets<CameraComponent>()) {
			camera->renderTarget->clear();
		}

		renderer->begin();
	}

	void RenderSystem::update(World& world, utl::DeltaTime deltaTime) {
		CLV_PROFILE_FUNCTION();

		//Transform and submit cameras
		for(auto&& [transform, camera] : world.getComponentSets<TransformComponent, CameraComponent>()) {
			const mth::vec3f& position = transform->getPosition();
			const mth::quatf cameraRotation = transform->getRotation();

			mth::vec3f eulerRot = mth::quaternionToEuler(cameraRotation);

			mth::vec3f front;
			front.x = sin(eulerRot.y) * cos(eulerRot.x);
			front.y = sin(eulerRot.x);
			front.z = cos(eulerRot.y) * cos(eulerRot.x);

			camera->cameraFront = mth::normalise(front);
			camera->cameraRight = mth::normalise(mth::cross(camera->cameraFront, mth::vec3f{ 0.0f, 1.0f, 0.0f }));
			camera->cameraUp = mth::normalise(mth::cross(camera->cameraRight, camera->cameraFront));

			const mth::mat4f lookAt = mth::lookAt(position, position + camera->cameraFront, camera->cameraUp);
			camera->currentView = lookAt;

			camera->cameraRenderData.lookAt = lookAt;
			camera->cameraRenderData.position = position;
			camera->cameraRenderData.projection = camera->currentProjection;

			renderer->submitCamera({ camera->viewport, camera->cameraRenderData, camera->renderTarget });
		}

		//Submit meshes
		for(auto&& [transform, renderable] : world.getComponentSets<TransformComponent, ModelComponent>()) {
			const mth::mat4f modelTransform = transform->getWorldTransformMatrix();

			for(auto& mesh : renderable->model.getMeshes()) {
				mesh->getMaterialInstance().setData(BBP_ModelData, VertexData{ modelTransform, mth::transpose(mth::inverse(modelTransform)) }, ShaderStage::Vertex);
				renderer->submitMesh(mesh);
			}
		}

		//Submit directional lights
		for(auto&& [light] : world.getComponentSets<DirectionalLightComponent>()) {
			light->lightData.shadowTransform = light->shadowProj * mth::lookAt(-mth::normalise(light->lightData.data.direction) * (light->farDist / 2.0f), mth::vec3f{ 0.0f, 0.0f, 0.0f }, mth::vec3f{ 0.0f, 1.0f, 0.0f });
			
			renderer->submitLight(light->lightData);
		}

		//Submit point lights
		for(auto&& [transform, light] : world.getComponentSets<TransformComponent, PointLightComponent>()) {
			const mth::vec3f& position = transform->getPosition();

			light->lightData.data.position = position;
			light->lightData.shadowTransforms[0] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{  1.0f,  0.0f,  0.0f }, mth::vec3f{  0.0f, -1.0f,  0.0f });
			light->lightData.shadowTransforms[1] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ -1.0f,  0.0f,  0.0f }, mth::vec3f{  0.0f, -1.0f,  0.0f });
			light->lightData.shadowTransforms[2] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{  0.0f,  1.0f,  0.0f }, mth::vec3f{  0.0f,  0.0f,  1.0f });
			light->lightData.shadowTransforms[3] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{  0.0f, -1.0f,  0.0f }, mth::vec3f{  0.0f,  0.0f, -1.0f });
			light->lightData.shadowTransforms[4] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{  0.0f,  0.0f,  1.0f }, mth::vec3f{  0.0f, -1.0f,  0.0f });
			light->lightData.shadowTransforms[5] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{  0.0f,  0.0f, -1.0f }, mth::vec3f{  0.0f, -1.0f,  0.0f });

			renderer->submitLight(light->lightData);
		}
	}

	void RenderSystem::postUpdate(World& world) {
		renderer->end();
	}
}