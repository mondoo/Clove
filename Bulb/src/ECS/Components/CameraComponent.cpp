#include "Bulb/ECS/Components/CameraComponent.hpp"

#include <Clove/Graphics/Surface.hpp>
#include <Clove/Platform/Window.hpp>

using namespace clv;

namespace blb::ecs {
	CameraComponent::CameraComponent(rnd::Camera camera)
		: camera(std::move(camera)) {
	}

	CameraComponent::CameraComponent(CameraComponent&& other) noexcept = default;

	CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept = default;

	CameraComponent::~CameraComponent() = default;

	void CameraComponent::setView(clv::mth::mat4f view) {
		camera.setView(std::move(view));
	}

	void CameraComponent::setProjectionMode(const rnd::ProjectionMode mode) {
		camera.setProjectionMode(mode);
	}

	void CameraComponent::setZoomLevel(float zoom) {
		camera.setZoomLevel(zoom);
	}

	void CameraComponent::setViewport(clv::gfx::Viewport viewport) {
		camera.setViewport(viewport);
	}

	const clv::mth::mat4f& CameraComponent::getView() const {
		return camera.getView();
	}

	const clv::mth::mat4f& CameraComponent::getProjection() const {
		return camera.getProjection();
	}

	rnd::ProjectionMode CameraComponent::getProjectionMode() const {
		return camera.getProjectionMode();
	}

	const std::shared_ptr<clv::gfx::RenderTarget>& CameraComponent::getRenderTarget() const {
		return camera.getRenderTarget();
	}

	const clv::gfx::Viewport& CameraComponent::getViewport() const {
		return camera.getViewport();
	}
}
