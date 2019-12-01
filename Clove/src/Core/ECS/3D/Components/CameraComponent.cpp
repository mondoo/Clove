#include "Clove/Core/ECS/3D/Components/CameraComponent.hpp"

#include "Clove/Core/Platform/Window.hpp"

namespace clv::ecs::_3D{
	CameraComponent::CameraComponent(const gfx::Viewport& viewport)
		: viewport(viewport){
		setProjectionMode(ProjectionMode::perspective);
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

	void CameraComponent::setProjectionMode(ProjectionMode mode){
		const float width = static_cast<float>(viewport.width);
		const float height = static_cast<float>(viewport.height);

		currentProjectionMode = mode;

		switch(currentProjectionMode){
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

	void CameraComponent::updateViewportSize(const mth::vec2ui& viewportSize){
		viewport.width = viewportSize.x;
		viewport.height = viewportSize.y;

		setProjectionMode(currentProjectionMode);
	}
}