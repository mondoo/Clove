#include "Bulb/Rendering/Camera.hpp"

#include <Clove/Platform/Window.hpp>

using namespace clv;
using namespace clv::gfx;

namespace blb::rnd {
    Camera::Camera(Viewport viewport, ProjectionMode const projection)
        : viewport(std::move(viewport)) {
        setProjectionMode(projection);
    }

    Camera::Camera(clv::plt::Window &window, ProjectionMode const projection) {
        windowResizeHandle = window.onWindowResize.bind([this](mth::vec2ui const &size) {
            setViewport({ 0, 0, static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) });
        });

        viewport = { 0, 0, window.getSize().x, window.getSize().y };
        setProjectionMode(projection);
    }

    Camera::Camera(Camera &&other) noexcept = default;

    Camera &Camera::operator=(Camera &&other) noexcept = default;

    Camera::~Camera() = default;

    void Camera::setView(clv::mth::mat4f view) {
        this->view = std::move(view);
    }

    void Camera::setProjectionMode(ProjectionMode const mode) {
        float constexpr orthographicSize = 15.0f;
        float const othoZoom             = orthographicSize * zoomLevel;

        float const width  = static_cast<float>(viewport.width);
        float const height = static_cast<float>(viewport.height);
        float const aspect = height > 0.0f ? width / height : 0.0f;

        float const nearPlane = 0.5f;
        float const farPlane  = 10000.0f;

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

    void Camera::setViewport(Viewport viewport) {
        this->viewport = std::move(viewport);
        setProjectionMode(currentProjectionMode);
    }

    clv::mth::mat4f const &Camera::getView() const {
        return view;
    }

    clv::mth::mat4f const &Camera::getProjection() const {
        return projection;
    }

    Camera::ProjectionMode Camera::getProjectionMode() const {
        return currentProjectionMode;
    }

    Viewport const &Camera::getViewport() const {
        return viewport;
    }
}