#include "Tunic/ECS/3D/Components/MeshComponent.hpp"

#include "Clove/Graphics/Core/Renderables/Mesh.hpp"

using namespace clv;

namespace tnc::ecs::_3D{
	MeshComponent::MeshComponent(const std::shared_ptr<gfx::Mesh>& mesh)
		: mesh(mesh){
	}

	MeshComponent::MeshComponent(const MeshComponent& other) = default;

	MeshComponent::MeshComponent(MeshComponent&& other) noexcept = default;

	MeshComponent& MeshComponent::operator=(const MeshComponent& other) = default;

	MeshComponent& MeshComponent::operator=(MeshComponent&& other) noexcept = default;

	MeshComponent::~MeshComponent() = default;
}