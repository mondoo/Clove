#include "clvpch.hpp"
#include "Camera.hpp"

namespace clv{
	Camera::Camera(){
		setProjectionMode(ProjectionMode::perspective);
	}

	void Camera::setPosition(const math::Vector3f& newPosition){
		cameraPosition = newPosition;
	}
	
	void Camera::update(float pitch, float yaw){
		//glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
		//glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));

		/*cameraDirection.y = sin(glm::radians(pitch));
		cameraDirection.x = cos(glm::radians(yaw));
		cameraDirection.z = cos(glm::radians(yaw));*/

		math::Vector3f front;
		front.x = cos(math::asRadians(yaw)) * cos(math::asRadians(pitch));
		front.y = sin(math::asRadians(pitch));
		front.z = sin(math::asRadians(yaw)) * cos(math::asRadians(pitch));
		cameraFront = math::normalise(front);
	}

	math::Matrix4f Camera::getLookAt() const{
		return math::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	}

	void Camera::setProjectionMode(ProjectionMode mode){
		switch(mode){
		case ProjectionMode::orthographic:
			currentProjection = math::createOrthographicMatrix(1.0f, -1.0f, -1.0f, 1.0f);
			break;

		case ProjectionMode::perspective:
			currentProjection = clv::math::createPerspectiveMatrix(45.0f, 16.0f / 9.0f, 1.0f, -1.0f);
			break;

		default:
			break;
		}
	}

	const math::Matrix4f& Camera::getProjection() const{
		return currentProjection;
	}
}