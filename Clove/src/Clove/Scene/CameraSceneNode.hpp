#pragma once

#include "SceneNode.hpp"

namespace clv{
	namespace scene{
		enum class ProjectionMode{
			orthographic,
			perspective
		};

		class CameraSceneNode : public SceneNode{
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
			CLV_API CameraSceneNode();

			CLV_API inline const math::Vector3f& getFront() const; //TODO: should take the roll and pitch and yaw or w/e
			CLV_API inline const math::Vector3f& getUp() const; //TODO: should probably be the camera's relative up
			CLV_API inline math::Vector3f getRight() const; //TODO: should use the getters (in the inl)

			CLV_API void update(float pitch, float yaw);

			//TODO: Need a get view function to handle both pers / ortho modes

			CLV_API math::Matrix4f getLookAt() const;

			CLV_API void setProjectionMode(ProjectionMode mode);
			CLV_API const math::Matrix4f& getProjection() const;
		};
	}
}

#include "CameraSceneNode.inl"