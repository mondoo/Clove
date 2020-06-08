#include "Bulb/ECS/Components/CameraComponent.hpp"

#include <Clove/Graphics/Surface.hpp>
#include <Clove/Platform/Window.hpp>

using namespace clv;

namespace blb::ecs {
	CameraComponent::CameraComponent(std::shared_ptr<clv::gfx::RenderTarget> renderTarget, const gfx::Viewport& viewport, const ProjectionMode projection)
		: renderTarget(std::move(renderTarget))
		, viewport(viewport) {
		setProjectionMode(projection);
	}

	CameraComponent::CameraComponent(plt::Window& window, const ProjectionMode projection) {
		viewport = { 0, 0, window.getSize().x, window.getSize().y };
		window.onWindowResize.bind(&CameraComponent::updateViewportSize, this);
		renderTarget = window.getSurface()->getRenderTarget();
		setProjectionMode(projection);
	}

	CameraComponent::CameraComponent(const CameraComponent& other) = default;

	CameraComponent::CameraComponent(CameraComponent&& other) noexcept = default;

	CameraComponent& CameraComponent::operator=(const CameraComponent& other) = default;

	CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept = default;

	CameraComponent::~CameraComponent() = default;

	const clv::mth::mat4f& CameraComponent::getProjection() const {
		
	}

	const clv::mth::mat4f& CameraComponent::getView() const {
		
	}

	const clv::gfx::Viewport& CameraComponent::getViewport() const {
		
	}

	void CameraComponent::setProjectionMode(const rnd::ProjectionMode mode) {
		
	}

	rnd::ProjectionMode CameraComponent::getProjectionMode() const {
		
	}

	void CameraComponent::setZoomLevel(float zoom){
		
	}

	void CameraComponent::updateViewportSize(const mth::vec2ui& viewportSize) {
		
	}
}
