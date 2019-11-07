#include "Material.hpp"

#include "Core/Graphics/Resources/Texture.hpp"
#include "Core/Graphics/MaterialInstance.hpp"
#include "Core/Graphics/RenderCommand.hpp"

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
		shader = RenderCommand::createShader({ shaderStyle });

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
		TextureDescriptor tdesc = {};
		tdesc.bindingPoint = TBP_Albedo;
		albedoTexture = RenderCommand::createTexture(tdesc);
		albedoTexture->map(path);
	}

	void Material::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Albedo, "Incorrect binding point for an albedo texture!");
		albedoTexture = texture;
	}

	void Material::setSpecularTexture(const std::string& path){
		TextureDescriptor tdesc = {};
		tdesc.bindingPoint = TBP_Specular;
		albedoTexture = RenderCommand::createTexture(tdesc);
		albedoTexture->map(path);
	}

	void Material::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Specular, "Incorrect binding point for a specular texture!");
		specTexture = texture;
	}
}