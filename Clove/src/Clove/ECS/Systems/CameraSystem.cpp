#include "clvpch.hpp"
#include "CameraSystem.hpp"

#include "Clove/Application.hpp"

namespace clv::ecs{
	CameraSystem::CameraSystem() = default;

	CameraSystem::CameraSystem(CameraSystem&& other) noexcept = default;

	CameraSystem& CameraSystem::operator=(CameraSystem&& other) noexcept = default;

	CameraSystem::~CameraSystem() = default;
	
	void CameraSystem::update(float deltaTime){
		for(auto& componentTuple : components){
			Transform3DComponent* transform = std::get<Transform3DComponent*>(componentTuple);
			CameraComponent* camera = std::get<CameraComponent*>(componentTuple);

			const math::Vector3f position = transform->getLocalPosition();

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

			camera->viewData.view = lookAt;
			camera->viewData.projection = camera->currentProjection;
			camera->sboMat->update(camera->viewData);

			camera->pos.pos = position;
			camera->sboPos->update(camera->pos);
		}
	}
}