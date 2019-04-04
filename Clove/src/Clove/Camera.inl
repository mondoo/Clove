namespace clv{
	inline const glm::vec3& Camera::getPosition() const{
		return cameraPosition;
	}

	inline CLV_API const glm::vec3& Camera::getFront() const{
		return cameraFront;
	}

	inline CLV_API const glm::vec3& Camera::getUp() const{
		return cameraUp;
	}

	inline CLV_API const glm::vec3& Camera::getRight() const{
		return glm::normalize(glm::cross(cameraFront, cameraUp));
	}
}