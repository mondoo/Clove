#include "clvpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace clv{
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

	math::Matrix4f Camera::getLookAt(){
		return math::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	}
}