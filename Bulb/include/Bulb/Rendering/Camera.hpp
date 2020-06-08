#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Bulb/Rendering/ShaderBufferTypes.hpp"

namespace clv::gfx {
	class RenderTarget;
}

namespace clv::plt {
	class Window;
}

namespace blb::rnd {
	enum class ProjectionMode {
		Orthographic,
		Perspective
	};
}

namespace blb::rnd {
	class Camera {
		//VARIABLES
	private:
		ProjectionMode currentProjectionMode;
		clv::mth::mat4f view	   = clv::mth::mat4f{ 1.0f };
		clv::mth::mat4f projection = clv::mth::mat4f{ 1.0f };

		std::shared_ptr<clv::gfx::RenderTarget> renderTarget;
		clv::gfx::Viewport viewport;

		float zoomLevel = 1.0f;

		//FUNCTIONS
	public:
		Camera() = delete;
		Camera(std::shared_ptr<clv::gfx::RenderTarget> renderTarget, const clv::gfx::Viewport& viewport, const ProjectionMode projection);
		Camera(clv::plt::Window& window, const ProjectionMode projection);

		Camera(const Camera& other);
		Camera(Camera&& other) noexcept;

		Camera& operator=(const Camera& other);
		Camera& operator=(Camera&& other) noexcept;

		~Camera();

		void setView(clv::mth::mat4f view);
		void setProjectionMode(const ProjectionMode mode);

		void setZoomLevel(float zoom);

		void setViewport(clv::gfx::Viewport viewport);

		const clv::mth::mat4f& getView() const;
		const clv::mth::mat4f& getProjection() const;

		ProjectionMode getProjectionMode() const;

		const clv::gfx::Viewport& getViewport() const;
	};
}