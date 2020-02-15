#include "Tunic/Rendering/MaterialInstance.hpp"

#include "Tunic/Rendering/Material.hpp"
#include "Clove/Graphics/Core/Resources/Texture.hpp"
#include "Clove/Graphics/Core/CommandBuffer.hpp"

using namespace clv::gfx;

namespace tnc::rnd{
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
	
	void MaterialInstance::bind(CommandBuffer& commandBuffer){
		if(albedoTexture){
			commandBuffer.bindTexture(albedoTexture.get(), TBP_Albedo);
		} else{
			commandBuffer.bindTexture(material->albedoTexture.get(), TBP_Albedo);
		}

		if(specTexture){
			commandBuffer.bindTexture(specTexture.get(), TBP_Specular);
		} else{
			commandBuffer.bindTexture(material->specTexture.get(), TBP_Specular);
		}

		for(auto& [bindingPoint, data] : material->shaderData){
			if(auto iter = shaderData.find(bindingPoint); iter == shaderData.end()){ //If we don't have data for that bindingPint
				commandBuffer.bindShaderResourceBuffer(*data.buffer, data.shaderType, bindingPoint);
			}
		}

		for(auto& [bindingPoint, data] : shaderData){
			commandBuffer.bindShaderResourceBuffer(*data.buffer, data.shaderType, bindingPoint);
		}
	}

	void MaterialInstance::setAlbedoTexture(const std::string& path){
		TextureDescriptor tdesc{};
		albedoTexture = Application::get().getGraphicsFactory().createTexture(tdesc, path);
	}

	void MaterialInstance::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		albedoTexture = texture;
	}

	void MaterialInstance::setSpecularTexture(const std::string& path){
		TextureDescriptor tdesc{};
		specTexture = Application::get().getGraphicsFactory().createTexture(tdesc, path);
	}

	void MaterialInstance::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		specTexture = texture;
	}
}