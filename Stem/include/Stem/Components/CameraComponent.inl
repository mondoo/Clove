namespace garlic::inline stem {
    void CameraComponent::setView(clv::mth::mat4f view) {
        camera.setView(std::move(view));
    }

    void CameraComponent::setProjectionMode(blb::rnd::Camera::ProjectionMode const mode) {
        camera.setProjectionMode(mode);
    }

    void CameraComponent::setZoomLevel(float zoom) {
        camera.setZoomLevel(zoom);
    }

    void CameraComponent::setViewport(blb::rnd::Viewport viewport) {
        camera.setViewport(viewport);
    }

    clv::mth::mat4f const &CameraComponent::getView() const {
        return camera.getView();
    }

    clv::mth::mat4f const &CameraComponent::getProjection() const {
        return camera.getProjection();
    }

    blb::rnd::Camera::ProjectionMode CameraComponent::getProjectionMode() const {
        return camera.getProjectionMode();
    }

    blb::rnd::Viewport const &CameraComponent::getViewport() const {
        return camera.getViewport();
    }
}