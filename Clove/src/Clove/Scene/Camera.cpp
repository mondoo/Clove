#include "clvpch.hpp"
#include "Camera.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindables/BindableFactory.hpp"

namespace clv::scene{
	Camera::Camera(){
		setProjectionMode(ProjectionMode::perspective);

		sboMat = gfx::BindableFactory::createShaderBufferObject<ViewData>(gfx::ShaderTypes::Vertex, 0u);
		sboMat->bind(Application::get().getWindow().getRenderer());

		sboPos = gfx::BindableFactory::createShaderBufferObject<ViewPos>(gfx::ShaderTypes::Pixel, 2u);
		sboPos->bind(Application::get().getWindow().getRenderer());
	}

	//Camera::Camera(const Camera& other) = default;

	Camera::Camera(Camera&& other) noexcept = default;

	//Camera& Camera::operator=(const Camera& other) = default;

	Camera& Camera::operator=(Camera&& other) noexcept = default;

	Camera::~Camera() = default;

	void Camera::update(float deltaSeconds){
		SceneNode::update(deltaSeconds);

		viewData.view = getLookAt();
		viewData.projection = currentProjection;
		sboMat->update(viewData, Application::get().getWindow().getRenderer());

		pos.pos = getPosition();
		sboPos->update(pos, Application::get().getWindow().getRenderer());
	}

	void Camera::updateFront(float pitch, float yaw){
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
		const math::Vector3f position = getPosition();
		return math::lookAt(getPosition(), position + cameraFront, cameraUp);
	}

	void Camera::setProjectionMode(ProjectionMode mode){
		switch(mode){
			case ProjectionMode::orthographic:
				currentProjection = math::createOrthographicMatrix(1.0f, -1.0f, -1.0f, 1.0f);
				break;

			case ProjectionMode::perspective:
				currentProjection = clv::math::createPerspectiveMatrix(45.0f, 16.0f / 9.0f, 0.5f, 50.0f);
				break;

			default:
				break;
		}
	}

	const math::Matrix4f& Camera::getProjection() const{
		return currentProjection;
	}
}