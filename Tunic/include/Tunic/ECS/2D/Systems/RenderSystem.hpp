#pragma once

#include "Tunic/ECS/Core/System.hpp"

namespace clv::plt{
	class Window;
}

namespace tnc::rnd{
	class Renderer2D;
}

namespace tnc::ecs::_2D{
	class RenderSystem : public System{
		//VARIABLES
	private:
		std::unique_ptr<rnd::Renderer2D> renderer;

		//FUNCTIONS
	public:
		RenderSystem() = delete;
		RenderSystem(std::unique_ptr<rnd::Renderer2D> renderer);
		RenderSystem(clv::plt::Window& window);

		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;

		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;

		virtual ~RenderSystem();

		virtual void preUpdate(World& world) override;
		virtual void update(World& world, clv::utl::DeltaTime deltaTime) override;
		virtual void postUpdate(World& world) override;
	};
}
