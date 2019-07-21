#pragma once

#include "Clove/ECS/Component.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs{
	class SpriteComponent : public Component{
		friend class Render2DSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x28fcf790; //VS Generated GUID

	private:
		gfx::SpriteRenderData submissionData;

		//FUNCTIONS
	public:
		SpriteComponent();
		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) noexcept;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) noexcept;
		virtual ~SpriteComponent();

		void setTexture(const std::string& path);
	};
}

