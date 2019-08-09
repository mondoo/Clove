#include "clvpch.hpp"
#include "Sprite.hpp"

#include "Clove/Graphics/Material.hpp"

namespace clv::gfx{
	Sprite::Sprite() = default;

	Sprite::Sprite(const Sprite& other) = default;

	Sprite& Sprite::operator=(const Sprite& other) = default;

	Sprite::Sprite(Sprite&& other) noexcept = default;

	Sprite& Sprite::operator=(Sprite&& other) noexcept = default;

	Sprite::~Sprite() = default;

	void Sprite::setMaterial(const std::shared_ptr<Material>& material){
		this->material = material;
	}

	void Sprite::bind(){
		material->bind();
	}
}