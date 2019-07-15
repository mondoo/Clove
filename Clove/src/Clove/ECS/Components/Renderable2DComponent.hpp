#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::gfx{
	class Texture;
}

namespace clv::ecs{
	class Renderable2DComponent : public Component{
		friend class Render2DSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x28fcf790; //VS Generated GUID

	private:
		std::shared_ptr<gfx::Texture> texture;

		//FUNCTIONS	
	public:
		Renderable2DComponent();
		Renderable2DComponent(const Renderable2DComponent& other) = delete;
		Renderable2DComponent(Renderable2DComponent&& other) noexcept;
		Renderable2DComponent& operator=(const Renderable2DComponent& other) = delete;
		Renderable2DComponent& operator=(Renderable2DComponent&& other) noexcept;
		virtual ~Renderable2DComponent();

	protected:
		void setTexture(const std::shared_ptr<gfx::Texture>& texture);
	};
}
