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
			RenderCommand::bindTexture(*albedoTexture, TBP_Albedo);
		} else if(material->albedoTexture){ //Thought this shouldn't be empty - but maybe it is?
			RenderCommand::bindTexture(*material->albedoTexture, TBP_Albedo);
		}

		if(specTexture){
			RenderCommand::bindTexture(*specTexture, TBP_Specular);
		} else if(material->specTexture){
			RenderCommand::bindTexture(*material->specTexture, TBP_Specular);
		}

		for(auto& [bindingPoint, data] : material->shaderData){
			if(auto iter = shaderData.find(bindingPoint); iter == shaderData.end()){ //If we don't have data for that bindingPint
				RenderCommand::bindShaderResourceBuffer(*data.buffer, data.shaderType, bindingPoint);
			}
		}

		for(auto& [bindingPoint, data] : shaderData){
			RenderCommand::bindShaderResourceBuffer(*data.buffer, data.shaderType, bindingPoint);
		}

		//RenderCommand::bindShader(*material->shader);
	}

	/*const ShaderReflectionData& MaterialInstance::getReflectionData() const{
		return material->getReflectionData();
	}

	const std::shared_ptr<Shader>& MaterialInstance::getShader() const{
		return material->getShader();
	}*/

	void MaterialInstance::setAlbedoTexture(const std::string& path){
		TextureDescriptor tdesc{};
		albedoTexture = RenderCommand::createTexture(tdesc, path);
	}

	void MaterialInstance::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		albedoTexture = texture;
	}

	void MaterialInstance::setSpecularTexture(const std::string& path){
		TextureDescriptor tdesc{};
		specTexture = RenderCommand::createTexture(tdesc, path);
	}

	void MaterialInstance::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		specTexture = texture;
	}
}