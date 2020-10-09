#include "Bulb/Rendering/Camera.hpp"

#include <Clove/Platform/Window.hpp>

using namespace clv;
using namespace clv::gfx;

namespace blb::rnd {
    Camera::Camera(clv::gfx::Viewport viewport, const ProjectionMode projection)
        : viewport(std::move(viewport)) {
        setProjectionMode(projection);
    }

    Camera::Camera(clv::plt::Window& window, const ProjectionMode projection) {
        windowResizeHandle = window.onWindowResize.bind([this](const mth::vec2ui& size) {
            setViewport({ 0, 0, static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) });
        });

        viewport = { 0, 0, window.getSize().x, window.getSize().y };
        setProjectionMode(projection);
    }

    Camera::Camera(Camera&& other) noexcept = default;

    Camera& Camera::operator=(Camera&& other) noexcept = default;

    Camera::~Camera() = default;

    void Camera::setView(clv::mth::mat4f view) {
        this->view = std::move(view);
    }

    void Camera::setProjectionMode(const ProjectionMode mode) {
        constexpr float orthographicSize = 15.0f;
        const float othoZoom             = orthographicSize * zoomLevel;

        const float width  = static_cast<float>(viewport.width);
        const float height = static_cast<float>(viewport.height);
        const float aspect = height > 0.0f ? width / height : 0.0f;

        const float nearPlane = 0.5f;
        const float farPlane  = 10000.0f;

        currentProjectionMode = mode;

        switch(currentProjectionMode) {
            case ProjectionMode::Orthographic:
                projection = mth::createOrthographicMatrix(-othoZoom * aspect, othoZoom * aspect, -othoZoom, othoZoom, nearPlane, farPlane);
                break;

            case ProjectionMode::Perspective:
                projection = mth::createPerspectiveMatrix(45.0f * zoomLevel, aspect, nearPlane, farPlane);
                break;

            default:
                break;
        }
    }

    void Camera::setZoomLevel(float zoom) {
        zoomLevel = zoom;
        setProjectionMode(currentProjectionMode);
    }

    void Camera::setViewport(clv::gfx::Viewport viewport) {
        this->viewport = std::move(viewport);
        setProjectionMode(currentProjectionMode);
    }

    const clv::mth::mat4f& Camera::getView() const {
        return view;
    }

    const clv::mth::mat4f& Camera::getProjection() const {
        return projection;
    }

    Camera::ProjectionMode Camera::getProjectionMode() const {
        return currentProjectionMode;
    }

    const clv::gfx::Viewport& Camera::getViewport() const {
        return viewport;
    }
}