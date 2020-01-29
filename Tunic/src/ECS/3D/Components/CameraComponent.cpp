#include "Tunic/ECS/3D/Components/CameraComponent.hpp"

#include "Clove/Platform/Core/Window.hpp"

using namespace clv;

namespace tnc::ecs::_3D{
	CameraComponent::CameraComponent(const gfx::Viewport& viewport, const ProjectionMode projection)
		: viewport(viewport){
		setProjectionMode(projection);
	}

	CameraComponent::CameraComponent(plt::Window& window, const ProjectionMode projection){
		viewport = { 0, 0, window.getSize().x, window.getSize().y };
		window.onWindowResize.bind(&CameraComponent::updateViewportSize, this);
		setProjectionMode(projection);
	}

	CameraComponent::CameraComponent(const CameraComponent& other) = default;

	CameraComponent::CameraComponent(CameraComponent&& other) noexcept = default;

	CameraComponent& CameraComponent::operator=(const CameraComponent& other) = default;

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

	void CameraComponent::setProjectionMode(const ProjectionMode mode){
		const float width = static_cast<float>(viewport.width);
		const float height = static_cast<float>(viewport.height);

		currentProjectionMode = mode;

		switch(currentProjectionMode){
			case ProjectionMode::orthographic:
				currentProjection = mth::createOrthographicMatrix(-(width / 2.0f), (width / 2.0f), -(height / 2.0f), (height / 2.0f));
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