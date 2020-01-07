#pragma once

#include "Tunic/ECS/Core/Component.hpp"

#include "Clove/Graphics/Core/ShaderBufferTypes.hpp"
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
		clv::mth::vec3f cameraFront = clv::mth::vec3f(0.0f, 0.0f, -1.0f);
		clv::mth::vec3f cameraRight = clv::mth::vec3f(-1.0f, 0.0f, 0.0f);

		clv::gfx::CameraRenderData cameraRenderData;

		std::shared_ptr<clv::gfx::RenderTarget> renderTarget;
		clv::gfx::Viewport viewport;

		//FUNCTIONS
	public:
		CameraComponent(const clv::gfx::Viewport& viewport);
		CameraComponent(clv::plt::Window& window);

		CameraComponent(const CameraComponent& other);
		CameraComponent(CameraComponent&& other) noexcept;

		CameraComponent& operator=(const CameraComponent& other);
		CameraComponent& operator=(CameraComponent&& other) noexcept;

		~CameraComponent();

		const clv::mth::vec3f& getFront() const;
		const clv::mth::vec3f& getUp() const;
		const clv::mth::vec3f& getRight() const;

		void setProjectionMode(ProjectionMode mode);

		void setRenderTarget(const std::shared_ptr<clv::gfx::RenderTarget>& renderTarget);

		void updateViewportSize(const clv::mth::vec2ui& viewportSize);
	};
}