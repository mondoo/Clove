#pragma once

#include "Core/ECS/System.hpp"

namespace clv::ecs::_3D{
	class RenderSystem : public System{
		//FUNCTIONS
	public:
		RenderSystem();

		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;

		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;

		virtual ~RenderSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}
