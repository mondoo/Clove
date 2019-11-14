#include "MaterialInstance.hpp"

#include "Core/Graphics/Material.hpp"
#include "Core/Graphics/Resources/Texture.hpp"
#include "Core/Graphics/RenderCommand.hpp"

namespace clv::gfx{
	MaterialInstance::MaterialInstance(std::shared_ptr<Material> material)
		: material(std::move(material)){
	}

	MaterialInstance::MaterialInstance(const MaterialInstance& other) = default;

	MaterialInstance::MaterialInstance(MaterialInstance&& other) noexcept{
		material = std::move(other.material);
		albedoTexture = std::move(other.albedoTexture);
		specTexture = std::move(other.specTexture);
		shaderData = std::move(other.shaderData);
	}

	MaterialInstance& MaterialInstance::operator=(const MaterialInstance& other) = default;

	MaterialInstance& MaterialInstance::operator=(MaterialInstance&& other) noexcept = default;

	MaterialInstance::~MaterialInstance() = default;
	
	void MaterialInstance::bind(){
		if(albedoTexture){
			RenderCommand::bindTexture(albedoTexture.get(), TBP_Albedo);
		} else if(material->albedoTexture){ //Thought this shouldn't be empty - but maybe it is?
			RenderCommand::bindTexture(material->albedoTexture.get(), TBP_Albedo);
		}

		if(specTexture){
			RenderCommand::bindTexture(specTexture.get(), TBP_Specular);
		} else if(material->specTexture){
			RenderCommand::bindTexture(material->specTexture.get(), TBP_Specular);
		}

		for(auto& [bindingPoint, data] : material->shaderData){
			if(auto iter = shaderData.find(bindingPoint); iter == shaderData.end()){ //If we don't have data for that bindingPint
				RenderCommand::bindShaderResourceBuffer(*data.buffer, data.shaderType, bindingPoint);
			}
		}

		for(auto& [bindingPoint, data] : shaderData){
			RenderCommand::bindShaderResourceBuffer(*data.buffer, data.shaderType, bindingPoint);
		}
	}

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