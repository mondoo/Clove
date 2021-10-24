#include "Clove/Rendering/Camera.hpp"

#include "Clove/Rendering/RenderingLog.hpp"

#include <Clove/Maths/MathsHelpers.hpp>

namespace clove {
    Camera::Camera(Viewport viewport, ProjectionMode const projection)
        : viewport{ viewport }
        , currentProjectionMode{ projection } {
        setProjectionMode(projection);
    }

    Camera::Camera(ProjectionMode const projection)
        : currentProjectionMode{ projection } {
        viewport = { 0.0f, 0.0f, 1.0f, 1.0f };
        setProjectionMode(projection);
    }

    mat4f Camera::getProjection(vec2ui const screenSize) const {
        float constexpr orthographicSize{ 15.0f };
        float constexpr fov{ 45.0f };
        float const othoZoom{ orthographicSize * zoomLevel };

        float const width{ static_cast<float>(screenSize.x) * viewport.width };
        float const height{ static_cast<float>(screenSize.y) * viewport.height };
        float const aspect{ height > 0.0f ? width / height : 0.0f };

        switch(currentProjectionMode) {
            case ProjectionMode::Orthographic:
                return createOrthographicMatrix(-othoZoom * aspect, othoZoom * aspect, -othoZoom, othoZoom, nearPlane, farPlane);
            case ProjectionMode::Perspective:
                return createPerspectiveMatrix(fov * zoomLevel, aspect, nearPlane, farPlane);
            default:
                CLOVE_ASSERT_MSG(false, "{0}: Case not handled", CLOVE_FUNCTION_NAME_PRETTY);
                return mat4f{ 1.0f };
        }
    }
}