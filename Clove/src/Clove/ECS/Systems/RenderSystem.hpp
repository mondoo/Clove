#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/TransformComponent.hpp"
#include "Clove/ECS/Components/RenderableComponent.hpp"

namespace clv::gfx{
	class Renderer;
}

namespace clv::ecs{
	class RenderSystem : public System<TransformComponent, RenderableComponent>{
		//VARIABLES
	public:
		static constexpr SystemID ID = 0x1ae78643; //VS generated GUID

	/*private:*/
	public://Public until i figure out this jank renderer shit
		//Should the render system own the renderer or just hold it?
		std::unique_ptr<gfx::Renderer> renderer; //TODO: Init renderer

		//FUNCTIONS
	public:
		RenderSystem();
		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;
		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;
		virtual ~RenderSystem();

		virtual void update(float deltaTime) override;
	};
}
