#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/3D/Components/TransformComponent.hpp"
#include "Clove/ECS/3D/Components/RenderableComponent.hpp"

namespace clv::ecs::d3{
	class RenderSystem : public System<TransformComponent, RenderableComponent>{
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
