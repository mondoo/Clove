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

		CameraComponent(const CameraComponent& other);
		CameraComponent(CameraComponent&& other) noexcept;

		CameraComponent& operator=(const CameraComponent& other);
		CameraComponent& operator=(CameraComponent&& other) noexcept;

		~CameraComponent();

		const clv::mth::mat4f& getProjection() const;
		const clv::mth::mat4f& getView() const;

		const clv::gfx::Viewport& getViewport() const;

		void setProjectionMode(const rnd::ProjectionMode mode);
		rnd::ProjectionMode getProjectionMode() const;

		void setZoomLevel(float zoom);

		void updateViewportSize(const clv::mth::vec2ui& viewportSize);
	};
}