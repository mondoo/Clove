#include "clvpch.hpp"
#include "RenderableComponent.hpp"

#include "Clove/Graphics/Bindable.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::ecs{
	RenderableComponent::RenderableComponent() = default;

	RenderableComponent::RenderableComponent(RenderableComponent&& other) noexcept = default;

	RenderableComponent& RenderableComponent::operator=(RenderableComponent&& other) noexcept = default;

	RenderableComponent::~RenderableComponent() = default;

	void RenderableComponent::addBindable(std::unique_ptr<gfx::Bindable> bindable){
		CLV_ASSERT(typeid(bindable) != typeid(gfx::IndexBuffer), "Index buffer found, please use Drawable::addIndexBuffer");
		CLV_ASSERT(typeid(bindable) != typeid(gfx::Shader), "Shaderr found, please use Drawable::addIndexBuffer");
		bindables.push_back(std::move(bindable));
	}

	void RenderableComponent::addIndexBuffer(std::unique_ptr<gfx::IndexBuffer> indexBuffer){
		CLV_ASSERT(this->indexBuffer == nullptr, "Index buffer already set!");
		this->indexBuffer = indexBuffer.get();
		bindables.push_back(std::move(indexBuffer));
	}

	void RenderableComponent::addShader(std::unique_ptr<gfx::Shader> shader){
		CLV_ASSERT(this->shader == nullptr, "Shader already set!");
		this->shader = shader.get();
		bindables.push_back(std::move(shader));
	}
}