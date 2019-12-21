#pragma once

#include "Clove/Core/ECS/Component.hpp"

namespace clv::gfx{
	class Mesh;
}

namespace clv::ecs::_3D{
	class MeshComponent : public Component<MeshComponent>{
		friend class RenderSystem;

		//VARIABLES
	private:
		std::shared_ptr<gfx::Mesh> mesh; 

		//FUNCTIONS
	public:
		MeshComponent() = delete;
		MeshComponent(const std::shared_ptr<gfx::Mesh>& mesh);

		MeshComponent(const MeshComponent& other);
		MeshComponent(MeshComponent&& other) noexcept;

		MeshComponent& operator=(const MeshComponent& other);
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		virtual ~MeshComponent();
	};
}