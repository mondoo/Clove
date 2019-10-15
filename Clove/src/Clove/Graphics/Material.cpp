#include "Material.hpp"

#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/MaterialInstance.hpp"

namespace clv::gfx{
	Material::Material(const Material& other) = default;

	Material& Material::operator=(const Material& other) = default;

	Material::Material(Material&& other) noexcept{
		shader = std::move(other.shader);
		albedoTexture = std::move(other.albedoTexture);
		specTexture = std::move(other.specTexture);
		shaderData = std::move(other.shaderData);
	}

	Material& Material::operator=(Material&& other) noexcept = default;

	Material::~Material() = default;

	Material::Material(ShaderStyle shaderStyle){
		shader = gfx::BindableFactory::createShader(shaderStyle);

		reflectionData = shader->getReflectionData();
	}

	MaterialInstance Material::createInstance(){
		return MaterialInstance{ shared_from_this() };
	}

	const ShaderReflectionData& Material::getReflectionData() const{
		return reflectionData;
	}

	const std::shared_ptr<Shader>& Material::getShader() const{
		return shader;
	}

	void Material::setAlbedoTexture(const std::string& path){
		albedoTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Albedo);
	}

	void Material::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Albedo, "Incorrect binding point for an albedo texture!");
		albedoTexture = texture;
	}

	void Material::setSpecularTexture(const std::string& path){
		specTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Specular);
	}

	void Material::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Specular, "Incorrect binding point for a specular texture!");
		specTexture = texture;
	}
}