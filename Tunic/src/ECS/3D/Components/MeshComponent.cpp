#include "Tunic/ECS/3D/Components/MeshComponent.hpp"

#include "Tunic/Rendering/Renderables/Mesh.hpp"

namespace tnc::ecs::_3D{
	MeshComponent::MeshComponent(std::shared_ptr<rnd::Mesh> mesh)
		: mesh(std::move(mesh)){
	}

	MeshComponent::MeshComponent(const MeshComponent& other) = default;

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept = default;

	MeshComponent& MeshComponent::operator=(const MeshComponent& other) = default;

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept = default;

	MeshComponent::~MeshComponent() = default;
}