#include "MaterialInstance.hpp"

#include "Core/Graphics/Material.hpp"
#include "Core/Graphics/Resources/Texture.hpp"
#include "Core/Graphics/RenderCommand.hpp"

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
			RenderCommand::bindTexture(*albedoTexture);
		} else { 
			RenderCommand::bindTexture(*material->albedoTexture);
		}

		if(specTexture){
			RenderCommand::bindTexture(*specTexture);
		} else if(material->specTexture){
			RenderCommand::bindTexture(*material->specTexture);
		}

		for(auto& [key, val] : material->shaderData){
			if(auto iter = shaderData.find(key); iter == shaderData.end()){
				RenderCommand::bindShaderResource(*val);
			}
		}

		for(auto& [key, val] : shaderData){
			RenderCommand::bindShaderResource(*val);
		}

		RenderCommand::bindShader(*material->shader);
	}

	const ShaderReflectionData& MaterialInstance::getReflectionData() const{
		return material->getReflectionData();
	}

	const std::shared_ptr<Shader>& MaterialInstance::getShader() const{
		return material->getShader();
	}

	void MaterialInstance::setAlbedoTexture(const std::string& path){
		TextureDescriptor tdesc = {};
		tdesc.bindingPoint = TBP_Albedo;
		albedoTexture = RenderCommand::createTexture(tdesc);
		albedoTexture->map(path);
	}

	void MaterialInstance::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Albedo, "Incorrect binding point for an albedo texture!");
		albedoTexture = texture;
	}

	void MaterialInstance::setSpecularTexture(const std::string& path){
		TextureDescriptor tdesc = {};
		tdesc.bindingPoint = TBP_Specular;
		albedoTexture = RenderCommand::createTexture(tdesc);
		albedoTexture->map(path);
	}

	void MaterialInstance::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		CLV_ASSERT(texture->getBindingPoint() == gfx::TBP_Specular, "Incorrect binding point for a specular texture!");
		specTexture = texture;
	}
}