#pragma once

#include "Clove/ECS/System.hpp"

#include "Clove/ECS/2D/Components/Transform2DComponent.hpp"
#include "Clove/ECS/2D/Components/SpriteComponent.hpp"

namespace clv::gfx{
	class Renderer;
	class Shader;
	class VertexBuffer;
	class IndexBuffer;
	class Bindable;
}

namespace clv::ecs{
	class Render2DSystem : public System<Transform2DComponent, SpriteComponent>{
		//FUNCTIONS
	public:
		Render2DSystem();
		Render2DSystem(const Render2DSystem& other) = delete;
		Render2DSystem(Render2DSystem&& other) noexcept;
		Render2DSystem& operator=(const Render2DSystem& other) = delete;
		Render2DSystem& operator=(Render2DSystem&& other) noexcept;
		virtual ~Render2DSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}
