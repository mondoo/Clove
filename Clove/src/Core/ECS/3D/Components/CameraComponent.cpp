#include "CameraComponent.hpp"

namespace clv::ecs::_3D{
	CameraComponent::CameraComponent(){
		setProjectionMode(ProjectionMode::perspective);
	}

	CameraComponent::CameraComponent(CameraComponent&& other) noexcept = default;

	CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept = default;

	CameraComponent::~CameraComponent() = default;

	const mth::vec3f& CameraComponent::getFront() const{
		return cameraFront;
	}

	const mth::vec3f& CameraComponent::getUp() const{
		return cameraUp;
	}

	const mth::vec3f& CameraComponent::getRight() const{
		return cameraRight;
	}

	void CameraComponent::updateFront(float pitch, float yaw){
		this->pitch = pitch;
		this->yaw = yaw;
	}

	void CameraComponent::setProjectionMode(ProjectionMode mode){
		switch(mode){
			case ProjectionMode::orthographic:
				//TODO: Need to get the window dimensions (ie. left = -(1920 / 2))
				currentProjection = mth::createOrthographicMatrix(-1.0f, 1.0f, -1.0f, 1.0f);
				break;

			case ProjectionMode::perspective:
				currentProjection = mth::createPerspectiveMatrix(45.0f, 16.0f / 9.0f, 0.5f, 10000.0f);
				break;

			default:
				break;
		}
	}

	void CameraComponent::setRenderTarget(const std::shared_ptr<gfx::RenderTarget>& renderTarget){
		this->renderTarget = renderTarget;
	}
}