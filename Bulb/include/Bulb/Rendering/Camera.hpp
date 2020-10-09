#pragma once

namespace clv::plt {
    class Window;
}

namespace blb::rnd {
    struct Viewport {
        int32_t x{ 0 };
        int32_t y{ 0 };
        int32_t width{ 0 };
        int32_t height{ 0 };
    };
}

namespace blb::rnd {
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
        clv::mth::mat4f view       = clv::mth::mat4f{ 1.0f };
        clv::mth::mat4f projection = clv::mth::mat4f{ 1.0f };

        Viewport viewport;

        float zoomLevel = 1.0f;

        clv::DelegateHandle windowResizeHandle;

        //FUNCTIONS
    public:
        Camera() = delete;
        Camera(Viewport viewport, const ProjectionMode projection);
        Camera(clv::plt::Window& window, const ProjectionMode projection);

        Camera(const Camera& other) = delete;
        Camera(Camera&& other) noexcept;

        Camera& operator=(const Camera& other) = delete;
        Camera& operator=(Camera&& other) noexcept;

        ~Camera();

        void setView(clv::mth::mat4f view);
        void setProjectionMode(const ProjectionMode mode);

        void setZoomLevel(float zoom);

        void setViewport(Viewport viewport);

        const clv::mth::mat4f& getView() const;
        const clv::mth::mat4f& getProjection() const;

        ProjectionMode getProjectionMode() const;

        const Viewport& getViewport() const;
    };
}