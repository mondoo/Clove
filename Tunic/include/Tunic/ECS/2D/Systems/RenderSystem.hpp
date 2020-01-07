#pragma once

#include "Tunic/ECS/Core/System.hpp"

namespace tnc::ecs::_2D{
	class RenderSystem : public System{
		//FUNCTIONS
	public:
		RenderSystem();

		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;

		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;

		virtual ~RenderSystem();

		virtual void preUpdate() override;
		virtual void update(clv::utl::DeltaTime deltaTime) override;
		virtual void postUpdate() override;
	};
}
