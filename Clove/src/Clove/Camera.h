#pragma once

namespace clv{
	class Camera{
		//VARIABLES
	private:
		glm::vec3 cameraPosition	= glm::vec3(0.0f, -30.0f, 0.0f);
		//glm::vec3 cameraRotation	= glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraTarget		= glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 cameraDirection	= glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraUp			= glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraRight		= glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraFront		= glm::vec3(0.0f, 0.0f, -1.0f);

		//FUNCTIONS
	public:
		CLV_API void setPosition(const glm::vec3& newPosition);
		CLV_API inline const glm::vec3& getPosition() const;

		CLV_API inline const glm::vec3& getFront() const; //TODO: should take the roll and pitch and yaw or w/e
		CLV_API inline const glm::vec3& getUp() const; //TODO: should probably be the camera's relative up
		CLV_API inline glm::vec3 getRight() const; //TODO: should use the getters (in the inl)

		CLV_API void update(float pitch, float yaw);

		CLV_API glm::mat4 getLookAt();
	};
}

#include "Clove/Camera.inl"