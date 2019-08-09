#include "clvpch.hpp"
#include "Sprite.hpp"

#include "Clove/Graphics/Material.hpp"

namespace clv::gfx{
	void Sprite::setMaterial(const std::shared_ptr<Material>& material){
		this->material = material;
	}

	void Sprite::bind(){
		material->bind();
	}
}