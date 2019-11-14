#include "Sprite.hpp"

#include "Core/Graphics/RenderCommand.hpp"
#include "Core/Graphics/Resources/Texture.hpp"
#include "Core/Graphics/Material.hpp"

namespace clv::gfx{
	Sprite::Sprite(){
		uint32 white = 0xffffffff;
		TextureDescriptor descriptor{};
		descriptor.dimensions = { 1, 1 };
		texture = RenderCommand::createTexture(descriptor, &white, 4);
	}

	Sprite::Sprite(const std::string& pathToTexture){
		texture = RenderCommand::createTexture({}, pathToTexture);
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

	void Sprite::setColour(const math::Vector4f& colour){
		this->colour = colour;
	}

	const math::Vector4f& Sprite::getColour() const{
		return colour;
	}

	void Sprite::setModelData(const math::Matrix4f& modelData){
		this->modelData = modelData;
	}

	const math::Matrix4f &Sprite::getModelData() const {
		return modelData;
	}
}