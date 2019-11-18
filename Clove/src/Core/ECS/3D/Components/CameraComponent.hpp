#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Graphics/ShaderBufferTypes.hpp"

namespace clv::ecs::_3D{
	enum class ProjectionMode{
		orthographic,
		perspective
	};

	class CameraComponent : public Component<CameraComponent>{
		friend class CameraSystem;

		//VARIABLES
	private:
		math::Matrix4f currentProjection = math::Matrix4f(1.0f);

		math::Vector3f cameraUp = math::Vector3f(0.0f, 1.0f, 0.0f);
		math::Vector3f cameraFront = math::Vector3f(0.0f, 0.0f, -1.0f);
		math::Vector3f cameraRight = math::Vector3f(-1.0f, 0.0f, 0.0f);

		float pitch = 0.0f;
		float yaw = 0.0f;

		gfx::CameraRenderData cameraRenderData;

		//FUNCTIONS
	public:
		CameraComponent();
		CameraComponent(const CameraComponent& other) = delete;
		CameraComponent(CameraComponent&& other) noexcept;
		CameraComponent& operator=(const CameraComponent& other) = delete;
		CameraComponent& operator=(CameraComponent&& other) noexcept;
		~CameraComponent();

		const math::Vector3f& getFront() const;
		const math::Vector3f& getUp() const;
		const math::Vector3f& getRight() const;

		void updateFront(float pitch, float yaw);

		void setProjectionMode(ProjectionMode mode);
	};
}