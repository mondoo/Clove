#pragma once

#include "Tunic/ECS/Core/System.hpp"

namespace clv::plt{
	class Window;
}

namespace tnc::rnd{
	class Renderer;
}

namespace tnc::ecs::_3D{
	class RenderSystem : public System{
		//VARIABLES
	private:
		std::unique_ptr<rnd::Renderer> renderer;

		//FUNCTIONS
	public:
		RenderSystem() = delete;
		RenderSystem(std::unique_ptr<rnd::Renderer> renderer);
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
