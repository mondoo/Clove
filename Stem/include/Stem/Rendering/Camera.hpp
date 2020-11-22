#pragma once

#include <Root/Delegate/DelegateHandle.hpp>

namespace clv::plt {
    class Window;
}

namespace garlic::inline stem {
    struct Viewport {
        int32_t x{ 0 };
        int32_t y{ 0 };
        int32_t width{ 0 };
        int32_t height{ 0 };
    };
}

namespace garlic::inline stem {
    class Camera {
        //TYPES
    public:
        enum class ProjectionMode {
            Orthographic,
            Perspective
        };

        //VARIABLES
    private:
        ProjectionMode currentProjectionMode;
        clv::mth::mat4f view{ 1.0f };
        clv::mth::mat4f projection{ 1.0f };

        Viewport viewport;

        float zoomLevel = 1.0f;

        garlic::DelegateHandle windowResizeHandle;

        //FUNCTIONS
    public:
        Camera() = delete;
        Camera(Viewport viewport, ProjectionMode const projection);
        Camera(ProjectionMode const projection);

        Camera(Camera const &other) = delete;
        Camera(Camera &&other) noexcept;

        Camera &operator=(Camera const &other) = delete;
        Camera &operator=(Camera &&other) noexcept;

        ~Camera();

        void setView(clv::mth::mat4f view);
        void setProjectionMode(ProjectionMode const mode);

        void setZoomLevel(float zoom);

        void setViewport(Viewport viewport);

        clv::mth::mat4f const &getView() const;
        clv::mth::mat4f const &getProjection() const;

        ProjectionMode getProjectionMode() const;

        Viewport const &getViewport() const;
    };
}