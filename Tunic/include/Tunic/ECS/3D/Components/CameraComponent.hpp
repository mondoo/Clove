#pragma once

#include "Tunic/ECS/Core/Component.hpp"

#include "Tunic/Rendering//ShaderBufferTypes.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"

namespace clv::gfx{
	class RenderTarget;
}

namespace clv::plt{
	class Window;
}

namespace tnc::ecs::_3D{
	enum class ProjectionMode{
		orthographic,
		perspective
	};
}

namespace tnc::ecs::_3D{
	class CameraComponent : public Component<CameraComponent>{
		friend class RenderSystem;

		//VARIABLES
	private:
		ProjectionMode currentProjectionMode;
		clv::mth::mat4f currentProjection = clv::mth::mat4f(1.0f);

		clv::mth::vec3f cameraUp = clv::mth::vec3f(0.0f, 1.0f, 0.0f);
		clv::mth::vec3f cameraFront = clv::mth::vec3f(0.0f, 0.0f, 1.0f);
		clv::mth::vec3f cameraRight = clv::mth::vec3f(1.0f, 0.0f, 0.0f);

		tnc::rnd::CameraRenderData cameraRenderData;

		std::shared_ptr<clv::gfx::RenderTarget> renderTarget;
		clv::gfx::Viewport viewport;

		//FUNCTIONS
	public:
		CameraComponent(std::shared_ptr<clv::gfx::RenderTarget> renderTarget, const clv::gfx::Viewport& viewport, const ProjectionMode projection);
		CameraComponent(clv::plt::Window& window, const ProjectionMode projection);

		CameraComponent(const CameraComponent& other);
		CameraComponent(CameraComponent&& other) noexcept;

		CameraComponent& operator=(const CameraComponent& other);
		CameraComponent& operator=(CameraComponent&& other) noexcept;

		~CameraComponent();

		const clv::mth::vec3f& getFront() const;
		const clv::mth::vec3f& getUp() const;
		const clv::mth::vec3f& getRight() const;

		void setProjectionMode(const ProjectionMode mode);

		void updateViewportSize(const clv::mth::vec2ui& viewportSize);
	};
}