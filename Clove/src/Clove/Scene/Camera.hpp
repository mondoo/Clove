#pragma once

#include "SceneNode.hpp"

namespace clv::scene{
	enum class ProjectionMode{
		orthographic,
		perspective
	};

	class Camera : public SceneNode{
		//VARIABLES
	private:
		math::Matrix4f currentProjection = math::Matrix4f(1.0f);

		//glm::vec3 cameraRotation		= glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraTarget		= glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 cameraDirection		= glm::vec3(0.0f, 0.0f, 0.0f);
		math::Vector3f cameraUp = math::Vector3f(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraRight			= glm::vec3(0.0f, 0.0f, 0.0f);
		math::Vector3f cameraFront = math::Vector3f(0.0f, 0.0f, -1.0f);

		//FUNCTIONS
	public:
		Camera();
		Camera(const Camera& other);
		Camera(Camera&& other) noexcept;
		Camera& operator=(const Camera& other);
		Camera& operator=(Camera&& other) noexcept;
		~Camera();

		inline const math::Vector3f& getFront() const; //TODO: should take the roll and pitch and yaw or w/e
		inline const math::Vector3f& getUp() const; //TODO: should probably be the camera's relative up
		inline math::Vector3f getRight() const; //TODO: should use the getters (in the inl)

		void updateFront(float pitch, float yaw);

		//TODO: Need a get view function to handle both pers / ortho modes

		math::Matrix4f getLookAt() const;

		void setProjectionMode(ProjectionMode mode);
		const math::Matrix4f& getProjection() const;
	};
}

#include "Camera.inl"