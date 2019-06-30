#include "clvpch.hpp"
#include "Renderable3DComponent.hpp"

#include "Clove/Graphics/Bindable.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::ecs{
	Renderable3DComponent::Renderable3DComponent() = default;

	Renderable3DComponent::Renderable3DComponent(Renderable3DComponent&& other) noexcept = default;

	Renderable3DComponent& Renderable3DComponent::operator=(Renderable3DComponent&& other) noexcept = default;

	Renderable3DComponent::~Renderable3DComponent() = default;

	void Renderable3DComponent::addBindable(std::unique_ptr<gfx::Bindable> bindable){
		CLV_ASSERT(typeid(bindable) != typeid(gfx::IndexBuffer), "Index buffer found, please use Drawable::addIndexBuffer");
		CLV_ASSERT(typeid(bindable) != typeid(gfx::Shader), "Shaderr found, please use Drawable::addIndexBuffer");
		bindables.push_back(std::move(bindable));
	}

	void Renderable3DComponent::addIndexBuffer(std::unique_ptr<gfx::IndexBuffer> indexBuffer){
		CLV_ASSERT(this->indexBuffer == nullptr, "Index buffer already set!");
		this->indexBuffer = indexBuffer.get();
		bindables.push_back(std::move(indexBuffer));
	}
}