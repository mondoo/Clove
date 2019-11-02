#include "MeshComponent.hpp"

#include "Core/Graphics/Mesh.hpp"

namespace clv::ecs::_3D{
	MeshComponent::MeshComponent() = default;

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept = default;

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept = default;

	MeshComponent::~MeshComponent() = default;

	void MeshComponent::setMesh(const std::shared_ptr<gfx::Mesh>& mesh){
		this->mesh = mesh;
	}
}