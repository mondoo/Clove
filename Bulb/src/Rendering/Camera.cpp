#include "Bulb/Rendering/Camera.hpp"

using namespace clv;
using namespace clv::gfx;

namespace blb::rnd {
	Camera::Camera(std::shared_ptr<clv::gfx::RenderTarget> renderTarget, const clv::gfx::Viewport& viewport, const ProjectionMode projection) {
	}

	Camera::Camera(clv::plt::Window& window, const ProjectionMode projection) {
	}

	Camera::Camera(const Camera& other) {
	}

	Camera::Camera(Camera&& other) noexcept {
	}

	Camera& Camera::operator=(const Camera& other) {

	}

	Camera& Camera::operator=(Camera&& other) noexcept{

	}

	Camera::~Camera(){

	}

	const clv::mth::mat4f& Camera::getProjection() const {
		return projection;
	}

	const clv::mth::mat4f& Camera::getView() const {
		return view;
	}

	const clv::gfx::Viewport& Camera::getViewport() const {
		return viewport;
	}

	void Camera::setProjectionMode(const ProjectionMode mode) {
		constexpr float orthographicSize = 15.0f;
		const float othoZoom			 = orthographicSize * zoomLevel;

		const float width  = static_cast<float>(viewport.width);
		const float height = static_cast<float>(viewport.height);
		const float aspect = width / height;

		const float nearPlane = 0.5f;
		const float farPlane  = 10000.0f;

		currentProjectionMode = mode;

		switch(currentProjectionMode) {
			case ProjectionMode::orthographic:
				currentProjection = mth::createOrthographicMatrix(-othoZoom * aspect, othoZoom * aspect, -othoZoom, othoZoom, nearPlane, farPlane);
				break;

			case ProjectionMode::perspective:
				currentProjection = mth::createPerspectiveMatrix(45.0f * zoomLevel, aspect, nearPlane, farPlane);
				break;

			default:
				break;
		}
	}

	ProjectionMode Camera::getProjectionMode() const {
		return currentProjectionMode;
	}

	void Camera::setZoomLevel(float zoom) {
		zoomLevel = zoom;

		setProjectionMode(currentProjectionMode);
	}

	void Camera::updateViewportSize(const mth::vec2ui& viewportSize) {
		viewport.width	= viewportSize.x;
		viewport.height = viewportSize.y;

		setProjectionMode(currentProjectionMode);
	}
}