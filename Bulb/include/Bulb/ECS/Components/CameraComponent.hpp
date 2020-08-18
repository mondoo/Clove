#pragma once

#include "Bulb/ECS/Component.hpp"
#include "Bulb/Rendering/Camera.hpp"

namespace blb::ecs {
    class CameraComponent : public Component<CameraComponent> {
        friend class RenderSystem;

        //VARIABLES
    private:
        rnd::Camera camera;

        //FUNCTIONS
    public:
        CameraComponent() = delete;
        CameraComponent(rnd::Camera camera);

        CameraComponent(const CameraComponent& other) = delete;
        CameraComponent(CameraComponent&& other) noexcept;

        CameraComponent& operator=(const CameraComponent& other) = delete;
        CameraComponent& operator=(CameraComponent&& other) noexcept;

        ~CameraComponent();

        void setView(clv::mth::mat4f view);
        void setProjectionMode(const rnd::ProjectionMode mode);

        void setZoomLevel(float zoom);

        void setViewport(clv::gfx::Viewport viewport);

        const clv::mth::mat4f& getView() const;
        const clv::mth::mat4f& getProjection() const;

        rnd::ProjectionMode getProjectionMode() const;

        const clv::gfx::Viewport& getViewport() const;
    };
}