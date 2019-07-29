#pragma once

#include "Clove/ECS/Component.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs::d2{
	class RenderableComponent : public Component{
		friend class RenderSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x28fcf790; //VS Generated GUID

	private:
		gfx::SpriteRenderData submissionData;

		//FUNCTIONS
	public:
		RenderableComponent();
		RenderableComponent(const RenderableComponent& other) = delete;
		RenderableComponent(RenderableComponent&& other) noexcept;
		RenderableComponent& operator=(const RenderableComponent& other) = delete;
		RenderableComponent& operator=(RenderableComponent&& other) noexcept;
		virtual ~RenderableComponent();

		void setTexture(const std::string& path);
	};
}

