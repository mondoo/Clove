#include "clvpch.hpp"
#include "MaterialInstance.hpp"

#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::gfx{
	MaterialInstance::MaterialInstance(const MaterialInstance& other) = default;

	MaterialInstance& MaterialInstance::operator=(const MaterialInstance& other) = default;

	MaterialInstance::MaterialInstance(MaterialInstance&& other) noexcept{
		material = std::move(other.material);
		albedoTexture = std::move(other.albedoTexture);
		specTexture = std::move(other.specTexture);
		shaderData = std::move(other.shaderData);
	}

	MaterialInstance& MaterialInstance::operator=(MaterialInstance&& other) noexcept = default;

	MaterialInstance::MaterialInstance(std::shared_ptr<Material> material)
		: material(std::move(material)){
	}

	void MaterialInstance::bind(){
		if(albedoTexture){
			albedoTexture->bind();
		} else if(material->albedoTexture){ //TODO: This shouldn't really be nullptr - but how will solid colour shaders work?
			material->albedoTexture->bind();
		}

		if(specTexture){
			specTexture->bind();
		} else if(material->specTexture){
			material->specTexture->bind();
		}

		for(auto& [key, val] : material->shaderData){
			if(!shaderData[key]){
				val->bind();
			}
		}

		for(auto& [key, val] : shaderData){
			val->bind();
		}

		material->bind();
	}

	const ShaderReflectionData& MaterialInstance::getReflectionData() const{
		return material->getReflectionData();
	}

	const std::shared_ptr<Shader>& MaterialInstance::getShader() const{
		return material->getShader();
	}

	void MaterialInstance::setAlbedoTexture(const std::string& path){
		albedoTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Albedo);
	}

	void MaterialInstance::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Albedo, "Incorrect binding point for an albedo texture!");
		albedoTexture = texture;
	}

	void MaterialInstance::setSpecularTexture(const std::string& path){
		specTexture = gfx::BindableFactory::createTexture(path, gfx::TBP_Specular);
	}

	void MaterialInstance::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Specular, "Incorrect binding point for a specular texture!");
		specTexture = texture;
	}
}