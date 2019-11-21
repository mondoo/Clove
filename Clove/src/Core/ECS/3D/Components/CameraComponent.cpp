#include "CameraComponent.hpp"

#include "Core/Platform/Window.hpp"

namespace clv::ecs::_3D{
	CameraComponent::CameraComponent(const mth::vec2ui& viewportSize){
		setProjectionMode(ProjectionMode::perspective, viewportSize);
	}

	CameraComponent::CameraComponent(plt::Window& window){
		window.onWindowResize.bind(&CameraComponent::onWindowSizeChanged, this);
		setProjectionMode(ProjectionMode::perspective, { window.getWidth(), window.getHeight() });
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

	void CameraComponent::setProjectionMode(ProjectionMode mode, const mth::vec2ui& viewportSize){
		const float width = static_cast<float>(viewportSize.x);
		const float height = static_cast<float>(viewportSize.y);

		switch(mode){
			case ProjectionMode::orthographic:
				currentProjection = mth::createOrthographicMatrix(-(width / 2), (width / 2), -(height / 2), (height / 2));
				break;

			case ProjectionMode::perspective:
				currentProjection = mth::createPerspectiveMatrix(45.0f, width / height, 0.5f, 10000.0f);
				break;

			default:
				break;
		}
	}

	void CameraComponent::setRenderTarget(const std::shared_ptr<gfx::RenderTarget>& renderTarget){
		this->renderTarget = renderTarget;
	}

	void CameraComponent::onWindowSizeChanged(const mth::vec2ui& viewport){
		setProjectionMode(ProjectionMode::perspective, viewport);
	}
}