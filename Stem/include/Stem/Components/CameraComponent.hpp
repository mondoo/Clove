#pragma once

#include <Bulb/ECS/Component.hpp>
#include <Bulb/Rendering/Camera.hpp>

namespace garlic::inline stem {
    class CameraComponent : public blb::ecs::Component<CameraComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        blb::rnd::Camera camera;

        //FUNCTIONS
    public:
        CameraComponent() = delete;
        CameraComponent(blb::rnd::Camera camera);

        CameraComponent(CameraComponent const &other) = delete;
        CameraComponent(CameraComponent &&other) noexcept;

        CameraComponent &operator=(CameraComponent const &other) = delete;
        CameraComponent &operator=(CameraComponent &&other) noexcept;

        ~CameraComponent();

        inline void setView(clv::mth::mat4f view);
        inline void setProjectionMode(blb::rnd::Camera::ProjectionMode const mode);

        inline void setZoomLevel(float zoom);

        inline void setViewport(blb::rnd::Viewport viewport);

        inline clv::mth::mat4f const &getView() const;
        inline clv::mth::mat4f const &getProjection() const;

        inline blb::rnd::Camera::ProjectionMode getProjectionMode() const;

        inline blb::rnd::Viewport const &getViewport() const;
    };
}

#include "CameraComponent.inl"