namespace garlic::clove {
    void CameraComponent::setView(mat4f view) {
        camera.setView(std::move(view));
    }

    void CameraComponent::setProjectionMode(Camera::ProjectionMode const mode) {
        camera.setProjectionMode(mode);
    }

    void CameraComponent::setZoomLevel(float zoom) {
        camera.setZoomLevel(zoom);
    }

    void CameraComponent::setViewport(Viewport viewport) {
        camera.setViewport(viewport);
    }

    mat4f const &CameraComponent::getView() const {
        return camera.getView();
    }

    mat4f const &CameraComponent::getProjection() const {
        return camera.getProjection();
    }

    Camera::ProjectionMode CameraComponent::getProjectionMode() const {
        return camera.getProjectionMode();
    }

    Viewport const &CameraComponent::getViewport() const {
        return camera.getViewport();
    }
}