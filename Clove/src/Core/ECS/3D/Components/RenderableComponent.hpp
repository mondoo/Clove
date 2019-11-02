#pragma once

#include "Core/ECS/Component.hpp"

#include "Core/Utils/HashString.hpp"

namespace clv::gfx{
	class Mesh;
}

namespace clv::ecs::_3D{
	class RenderableComponent : public Component{
		friend class RenderSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = HASH_CLASS(RenderableComponent);

	private:
		std::shared_ptr<gfx::Mesh> mesh; 

		//FUNCTIONS
	public:
		RenderableComponent();
		RenderableComponent(const RenderableComponent& other) = delete;
		RenderableComponent(RenderableComponent&& other) noexcept;
		RenderableComponent& operator=(const RenderableComponent& other) = delete;
		RenderableComponent& operator=(RenderableComponent&& other) noexcept;
		virtual ~RenderableComponent();

		void setMesh(const std::shared_ptr<gfx::Mesh>& mesh);
	};
}