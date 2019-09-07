#include "clvpch.hpp"
#include "Sprite.hpp"

#include "Clove/Graphics/Material.hpp"

namespace clv::gfx{
	Sprite::Sprite(const Sprite& other) = default;

	Sprite& Sprite::operator=(const Sprite& other) = default;

	Sprite::Sprite(Sprite&& other) noexcept = default;

	Sprite& Sprite::operator=(Sprite&& other) noexcept = default;

	Sprite::~Sprite() = default;

	Sprite::Sprite(MaterialInstance materialInstance)
		: materialInstance(std::move(materialInstance)){
	}

	MaterialInstance& Sprite::getMaterialInstance(){
		return materialInstance;
	}

	void Sprite::bind(){
		materialInstance.bind();
	}
}