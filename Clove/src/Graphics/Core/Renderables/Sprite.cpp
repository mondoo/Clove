#include "Clove/Graphics/Core/Renderables/Sprite.hpp"

#include "Clove/Graphics/Core/GraphicsGlobal.hpp"
#include "Clove/Graphics/Core/Resources/Texture.hpp"
#include "Clove/Graphics/Core/Material.hpp"

namespace clv::gfx{
	Sprite::Sprite(){
		uint32 white = 0xffffffff;
		TextureDescriptor descriptor{};
		descriptor.dimensions = { 1, 1 };
		texture = global::graphicsFactory->createTexture(descriptor, &white, 4);
	}

	Sprite::Sprite(const std::string& pathToTexture){
		texture = global::graphicsFactory->createTexture({}, pathToTexture);
	}

	Sprite::Sprite(std::shared_ptr<Texture> texture)
		: texture(std::move(texture)){
	}

	Sprite::Sprite(const Sprite& other) = default;

	Sprite::Sprite(Sprite&& other) noexcept = default;

	Sprite& Sprite::operator=(const Sprite& other) = default;

	Sprite& Sprite::operator=(Sprite&& other) noexcept = default;

	Sprite::~Sprite() = default;

	const std::shared_ptr<Texture> &Sprite::getTexture() const {
		return texture;
	}

	void Sprite::setColour(const mth::vec4f& colour){
		this->colour = colour;
	}

	const mth::vec4f& Sprite::getColour() const{
		return colour;
	}

	void Sprite::setModelData(const mth::mat4f& modelData){
		this->modelData = modelData;
	}

	const mth::mat4f &Sprite::getModelData() const {
		return modelData;
	}
}