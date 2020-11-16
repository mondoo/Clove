#pragma once

#include "Stem/Rendering/Camera.hpp"

#include <Bulb/ECS/Component.hpp>

namespace garlic::inline stem {
    class CameraComponent : public blb::ecs::Component<CameraComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        Camera camera;

        //FUNCTIONS
    public:
        CameraComponent() = delete;
        CameraComponent(Camera camera);

        CameraComponent(CameraComponent const &other) = delete;
        CameraComponent(CameraComponent &&other) noexcept;

        CameraComponent &operator=(CameraComponent const &other) = delete;
        CameraComponent &operator=(CameraComponent &&other) noexcept;

        ~CameraComponent();

        inline void setView(clv::mth::mat4f view);
        inline void setProjectionMode(Camera::ProjectionMode const mode);

        inline void setZoomLevel(float zoom);

        inline void setViewport(Viewport viewport);

        inline clv::mth::mat4f const &getView() const;
        inline clv::mth::mat4f const &getProjection() const;

        inline Camera::ProjectionMode getProjectionMode() const;

        inline Viewport const &getViewport() const;
    };
}

#include "CameraComponent.inl"