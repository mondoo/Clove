#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/Components/Transform2DComponent.hpp"
#include "Clove/ECS/Components/Renderable2DComponent.hpp"

namespace clv::gfx{
	class Renderer;
}

namespace clv::ecs{
	class Render2DSystem : public System<Transform2DComponent, Renderable2DComponent>{
		//VARIABLES
	public:
		static constexpr SystemID ID = 0xfa8540b2; //VS Generated GUID

	private:
		std::shared_ptr<gfx::Renderer> renderer;

		//FUNCTIONS
	public:
		Render2DSystem();
		Render2DSystem(const Render2DSystem& other) = delete;
		Render2DSystem(Render2DSystem&& other) noexcept;
		Render2DSystem& operator=(const Render2DSystem& other) = delete;
		Render2DSystem& operator=(Render2DSystem&& other) noexcept;
		virtual ~Render2DSystem();

		virtual void update(float deltaTime) override;

		void initialiseRenderer(const std::shared_ptr<gfx::Renderer>& renderer);
	};
}
