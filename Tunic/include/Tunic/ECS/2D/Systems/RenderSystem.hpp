#pragma once

#include "Tunic/ECS/Core/System.hpp"


namespace tnc::rnd{
	class Renderer2D;
}

namespace tnc::ecs::_2D{
	class RenderSystem : public System{
		//VARIABLES
	private:
		rnd::Renderer2D* renderer = nullptr;

		//FUNCTIONS
	public:
		RenderSystem(rnd::Renderer2D* renderer);

		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;

		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;

		virtual ~RenderSystem();

		virtual void preUpdate(World& world) override;
		virtual void update(World& world, clv::utl::DeltaTime deltaTime) override;
	};
}
