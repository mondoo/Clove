#include "clvpch.hpp"
#include "CameraSceneNode.hpp"

#include "Clove/Application.hpp"
#include "Clove/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"

namespace clv{
	namespace scene{
		CameraSceneNode::CameraSceneNode(){
			setProjectionMode(ProjectionMode::perspective);
		}

		CameraSceneNode::CameraSceneNode(const CameraSceneNode& other) = default;

		CameraSceneNode::CameraSceneNode(CameraSceneNode&& other) noexcept = default;

		CameraSceneNode::~CameraSceneNode() = default;

		void CameraSceneNode::updateFront(float pitch, float yaw){
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

		math::Matrix4f CameraSceneNode::getLookAt() const{
			const math::Vector3f position = getPosition();
			return math::lookAt(getPosition(), position + cameraFront, cameraUp);
		}

		void CameraSceneNode::setProjectionMode(ProjectionMode mode){
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

		const math::Matrix4f& CameraSceneNode::getProjection() const{
			return currentProjection;
		}

		CameraSceneNode& CameraSceneNode::operator=(const CameraSceneNode& other) = default;

		CameraSceneNode& CameraSceneNode::operator=(CameraSceneNode&& other) noexcept = default;
	}
}