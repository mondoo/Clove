#include "clvpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace clv{
	void Camera::setPosition(const glm::vec3& newPosition){
		cameraPosition = newPosition;
	}
	
	void Camera::update(float pitch, float yaw){
		//glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
		//glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));

		/*cameraDirection.y = sin(glm::radians(pitch));
		cameraDirection.x = cos(glm::radians(yaw));
		cameraDirection.z = cos(glm::radians(yaw));*/

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

	glm::mat4 Camera::getLookAt(){
		return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	}
}