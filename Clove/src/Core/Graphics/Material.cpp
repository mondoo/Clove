#include "Clove/Core/Graphics/Material.hpp"

#include "Clove/Core/Graphics/Resources/Texture.hpp"
#include "Clove/Core/Graphics/MaterialInstance.hpp"
#include "Clove/Core/Graphics/RenderCommand.hpp"

namespace clv::gfx{
	Material::Material(){
		uint32 white = 0xffffffff;
		TextureDescriptor descriptor{};
		descriptor.dimensions = { 1, 1 };
		auto blankTexture = RenderCommand::createTexture(descriptor, &white, 4);
		albedoTexture = blankTexture;
		specTexture = blankTexture;

		setData(BBP_Colour, mth::vec4f(1.0f, 1.0f, 1.0f, 1.0f), ShaderType::Pixel);
	}

	Material::Material(const Material& other) = default;

	Material::Material(Material&& other) noexcept{
		albedoTexture = std::move(other.albedoTexture);
		specTexture = std::move(other.specTexture);
		shaderData = std::move(other.shaderData);
	}

	Material& Material::operator=(const Material& other) = default;

	Material& Material::operator=(Material&& other) noexcept = default;

	Material::~Material() = default;

	MaterialInstance Material::createInstance(){
		return MaterialInstance{ shared_from_this() };
	}

	void Material::setAlbedoTexture(const std::string& path){
		TextureDescriptor tdesc{};
		albedoTexture = RenderCommand::createTexture(tdesc, path);
	}

	void Material::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		albedoTexture = texture;
	}

	void Material::setSpecularTexture(const std::string& path){
		TextureDescriptor tdesc{};
		specTexture = RenderCommand::createTexture(tdesc, path);
	}

	void Material::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		specTexture = texture;
	}
}