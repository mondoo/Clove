#pragma once

#include "Clove/Rendering/Camera.hpp"

#include <Clove/ECS/Component.hpp>
#include <Clove/Maths/Matrix.hpp>

namespace garlic::clove {
    class CameraComponent : public Component<CameraComponent> {
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

        inline float getNearPlane() const;
        inline float getFarPlane() const;

        inline void setView(mat4f view);
        inline void setProjectionMode(Camera::ProjectionMode const mode);

        inline void setZoomLevel(float zoom);

        inline void setViewport(Viewport viewport);

        inline mat4f const &getView() const;
        inline mat4f const &getProjection() const;

        inline Camera::ProjectionMode getProjectionMode() const;

        inline Viewport const &getViewport() const;
    };
}

#include "CameraComponent.inl"