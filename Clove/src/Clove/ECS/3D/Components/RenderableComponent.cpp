#include "clvpch.hpp"
#include "RenderableComponent.hpp"

#include "Clove/Graphics/Mesh.hpp"

namespace clv::ecs::d3{
	RenderableComponent::RenderableComponent() = default;

	RenderableComponent::RenderableComponent(RenderableComponent&& other) noexcept = default;

	RenderableComponent& RenderableComponent::operator=(RenderableComponent&& other) noexcept = default;

	RenderableComponent::~RenderableComponent() = default;

	void RenderableComponent::setMesh(const std::string& filePath){
		mesh = std::make_unique<gfx::Mesh>(filePath);
	}

	void RenderableComponent::setMesh(std::unique_ptr<gfx::Mesh> mesh){
		this->mesh = std::move(mesh);
	}
}