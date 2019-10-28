#include "RenderableComponent.hpp"

#include "Core/Graphics/Mesh.hpp"

namespace clv::ecs::_3D{
	RenderableComponent::RenderableComponent() = default;

	RenderableComponent::RenderableComponent(RenderableComponent&& other) noexcept = default;

	RenderableComponent& RenderableComponent::operator=(RenderableComponent&& other) noexcept = default;

	RenderableComponent::~RenderableComponent() = default;

	void RenderableComponent::setMesh(const std::shared_ptr<gfx::Mesh>& mesh){
		this->mesh = mesh;
	}
}