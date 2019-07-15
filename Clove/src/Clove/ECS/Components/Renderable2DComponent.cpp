#include "clvpch.hpp"
#include "Renderable2DComponent.hpp"

#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::ecs{
	Renderable2DComponent::Renderable2DComponent() = default;

	Renderable2DComponent::Renderable2DComponent(Renderable2DComponent&& other) noexcept = default;

	Renderable2DComponent& Renderable2DComponent::operator=(Renderable2DComponent&& other) noexcept = default;

	Renderable2DComponent::~Renderable2DComponent() = default;

	//Temp - need bindables as shared_ptrs
	/*void Renderable2DComponent::setTexture(std::unique_ptr<gfx::Texture> texture){
		this->texture = std::move(texture);
	}*/
}