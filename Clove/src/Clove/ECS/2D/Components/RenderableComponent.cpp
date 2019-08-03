#include "clvpch.hpp"
#include "RenderableComponent.hpp"

#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::ecs::d2{
	RenderableComponent::RenderableComponent() = default;

	RenderableComponent::RenderableComponent(RenderableComponent&& other) noexcept = default;

	RenderableComponent& RenderableComponent::operator=(RenderableComponent&& other) noexcept = default;

	RenderableComponent::~RenderableComponent() = default;

	void RenderableComponent::setTexture(const std::string& path){
		submissionData.texture = gfx::BindableFactory::createTexture(path, gfx::TBP_Albedo);
	}
}