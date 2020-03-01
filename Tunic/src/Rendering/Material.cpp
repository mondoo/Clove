#include "Tunic/Rendering/Material.hpp"

#include "Tunic/Rendering/MaterialInstance.hpp"
#include "Tunic/Rendering/ShaderBufferTypes.hpp"
#include "Clove/Graphics/Core/Resources/Texture.hpp"

using namespace clv::gfx;

namespace tnc::rnd{
	Material::Material(){
		uint32_t white = 0xffffffff;
		TextureDescriptor descriptor{};
		descriptor.dimensions = { 1, 1 };
		auto blankTexture = Application::get().getGraphicsFactory().createTexture(descriptor, &white, 4);
		albedoTexture = blankTexture;
		specTexture = blankTexture;

		setData(BBP_Colour, clv::mth::vec4f(1.0f, 1.0f, 1.0f, 1.0f), ShaderStage::Pixel);
		setData(BBP_MaterialData, MaterialData{ 32.0f }, ShaderStage::Pixel);
	}

	Material::Material(const Material& other) = default;

	Material::Material(Material&& other) noexcept{
		albedoTexture	= std::move(other.albedoTexture);
		specTexture		= std::move(other.specTexture);
		shaderData		= std::move(other.shaderData);
	}

	Material& Material::operator=(const Material& other) = default;

	Material& Material::operator=(Material&& other) noexcept = default;

	Material::~Material() = default;

	MaterialInstance Material::createInstance(){
		return MaterialInstance{ shared_from_this() };
	}

	void Material::setAlbedoTexture(const std::string& path){
		TextureDescriptor tdesc{};
		albedoTexture = Application::get().getGraphicsFactory().createTexture(tdesc, path);
	}

	void Material::setAlbedoTexture(std::shared_ptr<Texture> texture){
		albedoTexture = std::move(texture);
	}

	void Material::setSpecularTexture(const std::string& path){
		TextureDescriptor tdesc{};
		specTexture = Application::get().getGraphicsFactory().createTexture(tdesc, path);
	}

	void Material::setSpecularTexture(std::shared_ptr<Texture> texture){
		specTexture = std::move(texture);
	}
}