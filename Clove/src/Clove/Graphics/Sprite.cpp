#include "Sprite.hpp"

#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::gfx{
	Sprite::Sprite(const Sprite& other) = default;

	Sprite& Sprite::operator=(const Sprite& other) = default;

	Sprite::Sprite(Sprite&& other) noexcept = default;

	Sprite& Sprite::operator=(Sprite&& other) noexcept = default;

	Sprite::~Sprite() = default;

	Sprite::Sprite(const std::string& pathToTexture){
		texture = BindableFactory::createTexture(pathToTexture, TBP_Albedo);
	}

	Sprite::Sprite(std::shared_ptr<Texture> texture)
		: texture(std::move(texture)){
	}

	const std::shared_ptr<Texture> &Sprite::getTexture() const {
		return texture;
	}

	void Sprite::setModelData(const math::Matrix4f& modelData){
		this->modelData = modelData;
	}

	const math::Matrix4f &Sprite::getModelData() const {
		return modelData;
	}
}