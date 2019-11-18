#include "CameraSystem.hpp"

#include "Core/ECS/Manager.hpp"
#include "Core/ECS/3D/Components/TransformComponent.hpp"
#include "Core/ECS/3D/Components/CameraComponent.hpp"
#include "Core/Graphics/Renderer.hpp"

namespace clv::ecs::_3D{
	CameraSystem::CameraSystem() = default;

	CameraSystem::CameraSystem(CameraSystem&& other) noexcept = default;

	CameraSystem& CameraSystem::operator=(CameraSystem&& other) noexcept = default;

	CameraSystem::~CameraSystem() = default;
	
	void CameraSystem::update(utl::DeltaTime deltaTime){
		auto componentTuples = manager->getComponentSets<TransformComponent, CameraComponent>();
		for(auto& tuple : componentTuples){
			TransformComponent* transform = std::get<TransformComponent*>(tuple);
			CameraComponent* camera = std::get<CameraComponent*>(tuple);

			const math::Vector3f& position = transform->getPosition();

			//update front
			math::Vector3f front;
			front.x = cos(math::asRadians(camera->yaw)) * cos(math::asRadians(camera->pitch));
			front.y = sin(math::asRadians(camera->pitch));
			front.z = sin(math::asRadians(camera->yaw)) * cos(math::asRadians(camera->pitch));
			camera->cameraFront = math::normalise(front);

			//update look at
			const math::Matrix4f lookAt = math::lookAt(position, position + camera->cameraFront, camera->cameraUp);

			//update right
			camera->cameraRight = math::normalise(math::cross(camera->cameraFront, camera->cameraUp));

			camera->cameraRenderData.lookAt = lookAt;
			camera->cameraRenderData.position = position;
			camera->cameraRenderData.projection = camera->currentProjection;

			//Assuming a single camera. but will just use the last to update if there are multiple
			//will need to change
			gfx::Renderer::setCamera(camera->cameraRenderData);
		}
	}
}