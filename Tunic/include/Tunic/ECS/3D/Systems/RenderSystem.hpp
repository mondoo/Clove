#pragma once

#include "Tunic/ECS/Core/System.hpp"

namespace tnc::rnd{
	class Renderer;
}

namespace tnc::ecs::_3D{
	class RenderSystem : public System{
		//VARIABLES
	private:
		rnd::Renderer* renderer = nullptr;

		//FUNCTIONS
	public:
		RenderSystem() = delete;
		RenderSystem(rnd::Renderer* renderer);

		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;

		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;

		virtual ~RenderSystem();

		virtual void preUpdate() override;
		virtual void update(clv::utl::DeltaTime deltaTime) override;
	};
}
