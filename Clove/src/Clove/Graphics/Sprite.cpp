#include "clvpch.hpp"
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
		initialise();
	}

	Sprite::Sprite(std::shared_ptr<Texture> texture)
		: texture(std::move(texture)){
		initialise();
	}

	void Sprite::bind(){
		texture->bind();
		modelData->bind();
	}

	void Sprite::setModelData(const math::Matrix4f& modelData){
		this->modelData->update(modelData);
	}

	void Sprite::initialise(){
		modelData = BindableFactory::createShaderBufferObject<math::Matrix4f>(ShaderType::Vertex, BBP_2DData);
	}
}