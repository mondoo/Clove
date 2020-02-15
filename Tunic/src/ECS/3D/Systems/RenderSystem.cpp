#include "Tunic/ECS/3D/Systems/RenderSystem.hpp"

#include "Tunic/ECS/Core/World.hpp"
#include "Tunic/ECS/3D/Components/TransformComponent.hpp"
#include "Tunic/ECS/3D/Components/MeshComponent.hpp"
#include "Tunic/ECS/3D/Components/CameraComponent.hpp"
#include "Tunic/ECS/3D/Components/LightComponent.hpp"
#include "Tunic/Rendering/Renderables/Mesh.hpp"
#include "Tunic/Rendering/Renderer.hpp"

#include <Clove/Graphics/Core/Resources/Texture.hpp>
#include <Clove/Graphics/Core/CommandBuffer.hpp>
#include <Clove/Graphics/Core/GraphicsFactory.hpp>
#include <Clove/Graphics/Core/PipelineObject.hpp>
#include <Clove/Platform/Core/Window.hpp>
#include <Clove/Graphics/Core/RenderTarget.hpp>

using namespace clv;
using namespace clv::gfx;
using namespace tnc::rnd;

namespace tnc::ecs::_3D{
	RenderSystem::RenderSystem(rnd::Renderer* renderer)
		: renderer(renderer){
	}

	RenderSystem::RenderSystem(RenderSystem&& other) noexcept = default;

	RenderSystem& RenderSystem::operator=(RenderSystem&& other) noexcept = default;

	RenderSystem::~RenderSystem() = default;

	void RenderSystem::preUpdate(World& world){
		auto componentTuples = world.getComponentSets<CameraComponent>();
		for (auto& tuple : componentTuples){
			CameraComponent* camera = std::get<CameraComponent*>(tuple);
			camera->renderTarget->clear();
		}
	}

	void RenderSystem::update(World& world, utl::DeltaTime deltaTime){
		CLV_PROFILE_FUNCTION();

		//Transform and submit cameras
		{
			auto componentTuples = world.getComponentSets<TransformComponent, CameraComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				CameraComponent* camera = std::get<CameraComponent*>(tuple);

				const mth::vec3f& position = transform->getPosition();
				const mth::quatf cameraRotation = transform->getRotation();

				mth::vec3f eulerRot = mth::quaternionToEuler(cameraRotation);

				mth::vec3f front;
				front.x = sin(eulerRot.y) * cos(eulerRot.x);
				front.y = sin(eulerRot.x);
				front.z = cos(eulerRot.y) * cos(eulerRot.x);

				camera->cameraFront = mth::normalise(front);
				camera->cameraRight = mth::normalise(mth::cross(camera->cameraFront, mth::vec3f{ 0.0f, 1.0f, 0.0f }));
				camera->cameraUp	= mth::normalise(mth::cross(camera->cameraRight, camera->cameraFront));

				const mth::mat4f lookAt = mth::lookAt(position, position + camera->cameraFront, camera->cameraUp);

				camera->cameraRenderData.lookAt 	= lookAt;
				camera->cameraRenderData.position 	= position;
				camera->cameraRenderData.projection = camera->currentProjection;

				renderer->submitCamera({ camera->viewport, camera->cameraRenderData, camera->renderTarget });
			}
		}

		//Submit meshes
		{
			auto componentTuples = world.getComponentSets<TransformComponent, MeshComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				MeshComponent* renderable = std::get<MeshComponent*>(tuple);

				const mth::mat4f model = transform->getWorldTransformMatrix();
				renderable->mesh->getMaterialInstance().setData(BBP_ModelData, VertexData{ model, mth::transpose(mth::inverse(model)) }, ShaderStage::Vertex);

				renderer->submitMesh(renderable->mesh);
			}
		}

		//Submit lights
		{
			auto componentTuples = world.getComponentSets<TransformComponent, LightComponent>();
			for(auto& tuple : componentTuples){
				TransformComponent* transform = std::get<TransformComponent*>(tuple);
				LightComponent* light = std::get<LightComponent*>(tuple);

				const auto& position = transform->getPosition();

				light->lightData.intensity.position = position;
				light->lightData.shadowTransforms[0] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 1.0, 0.0, 0.0 }, mth::vec3f{ 0.0,-1.0, 0.0 });
				light->lightData.shadowTransforms[1] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ -1.0, 0.0, 0.0 }, mth::vec3f{ 0.0,-1.0, 0.0 });
				light->lightData.shadowTransforms[2] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0, 1.0, 0.0 }, mth::vec3f{ 0.0, 0.0, 1.0 });
				light->lightData.shadowTransforms[3] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0,-1.0, 0.0 }, mth::vec3f{ 0.0, 0.0,-1.0 });
				light->lightData.shadowTransforms[4] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0, 0.0, 1.0 }, mth::vec3f{ 0.0,-1.0, 0.0 });
				light->lightData.shadowTransforms[5] = light->shadowProj * mth::lookAt(position, position + mth::vec3f{ 0.0, 0.0,-1.0 }, mth::vec3f{ 0.0,-1.0, 0.0 });

				renderer->submitLight({ light->lightData.intensity, light->lightData.shadowTransforms, light->lightData.farPlane });
			}
		}
	}
}