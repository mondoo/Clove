#include "clvpch.hpp"
#include "Material.hpp"

#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::gfx{
	Material::Material() = default;

	Material::Material(const Material& other) = default;

	Material& Material::operator=(const Material& other) = default;

	Material::Material(Material&& other) noexcept{
		modelData = std::move(other.modelData);

		albedoTexture = std::move(other.albedoTexture);
		specTexture = std::move(other.specTexture);
		shaderData = std::move(other.shaderData);
	}

	Material& Material::operator=(Material&& other) noexcept = default;

	Material::~Material() = default;

	void Material::bind(){
		//shader->bind();
		albedoTexture->bind();
		if(specTexture){
			specTexture->bind();
		}
		for(auto& [key, val] : shaderData){
			val->bind();
		}
	}

	void Material::setAlbedoTexture(const std::string& path){
		albedoTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Albedo);
	}

	void Material::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		albedoTexture = texture;
	}

	void Material::setSpecularTexture(const std::string& path){
		specTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Specular);
	}

	void Material::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		specTexture = texture;
	}
}