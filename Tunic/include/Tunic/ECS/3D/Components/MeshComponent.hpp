#pragma once

#include "Tunic/ECS/Core/Component.hpp"

namespace clv::gfx{
	class Mesh;
}

namespace tnc::ecs::_3D{
	class MeshComponent : public Component<MeshComponent>{
		friend class RenderSystem;

		//VARIABLES
	private:
		std::shared_ptr<clv::gfx::Mesh> mesh; 

		//FUNCTIONS
	public:
		MeshComponent() = delete;
		MeshComponent(const std::shared_ptr<clv::gfx::Mesh>& mesh);

		MeshComponent(const MeshComponent& other);
		MeshComponent(MeshComponent&& other) noexcept;

		MeshComponent& operator=(const MeshComponent& other);
		MeshComponent& operator=(MeshComponent&& other) noexcept;

		virtual ~MeshComponent();
	};
}