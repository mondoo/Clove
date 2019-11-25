#pragma once

#include "Core/ECS/System.hpp"

namespace clv::ecs::_2D{
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
		virtual void update(utl::DeltaTime deltaTime) override;
		virtual void postUpdate() override;

	private:
		void onWindowSizeChanged(const mth::vec2ui& size);
	};
}
