#pragma once

#include "Clove/ECS/Component.hpp"

namespace clv::gfx{
	class Mesh;
}

namespace clv::ecs::d3{
	class RenderableComponent : public Component{
		friend class RenderSystem;

		//VARIABLES
	public:
		static constexpr ComponentID ID = 0x7dfb2c10; //VS Generated GUID

	private:
		std::shared_ptr<gfx::Mesh> mesh; 
		//shared ptr? then things can use the same mesh?
		//unique_ptr? do we want to manage the mesh another way?
		//textures can be stored in a material?

		//FUNCTIONS
	public:
		RenderableComponent();
		RenderableComponent(const RenderableComponent& other) = delete;
		RenderableComponent(RenderableComponent&& other) noexcept;
		RenderableComponent& operator=(const RenderableComponent& other) = delete;
		RenderableComponent& operator=(RenderableComponent&& other) noexcept;
		virtual ~RenderableComponent();

		void setMesh(const std::string& filePath);
		void setMesh(const std::shared_ptr<gfx::Mesh>& mesh);
	};
}