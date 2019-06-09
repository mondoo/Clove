#include "clvpch.hpp"
#include "Drawable.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Bindable.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"

namespace clv::gfx{
	Drawable::Drawable() = default;

	Drawable::Drawable(Drawable&& other) noexcept = default;

	Drawable& Drawable::operator=(Drawable&& other) noexcept = default;

	Drawable::~Drawable() = default;

	void Drawable::draw(Renderer& renderer){
		for(auto& bindable : bindables){
			bindable->bind(renderer);
		}

		renderer.drawIndexed(indexBuffer->getIndexCount());
	}

	void Drawable::addBindable(std::unique_ptr<Bindable> bindable){
		CLV_ASSERT(typeid(bindable) != typeid(IndexBuffer), "Index buffer found, please use Drawable::addIndexBuffer");
		bindables.push_back(std::move(bindable));
	}

	void Drawable::addIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer){
		CLV_ASSERT(this->indexBuffer == nullptr, "Index buffer already set!");
		this->indexBuffer = indexBuffer.get();
		bindables.push_back(std::move(indexBuffer));
	}
}