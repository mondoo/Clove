#include "Clove/Core/Graphics/MaterialInstance.hpp"

#include "Clove/Core/Graphics/Material.hpp"
#include "Clove/Core/Graphics/Resources/Texture.hpp"
#include "Clove/Core/Graphics/GraphicsGlobal.hpp"

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
			global::graphicsDevice->bindTexture(albedoTexture.get(), TBP_Albedo);
		} else{
			global::graphicsDevice->bindTexture(material->albedoTexture.get(), TBP_Albedo);
		}

		if(specTexture){
			global::graphicsDevice->bindTexture(specTexture.get(), TBP_Specular);
		} else{
			global::graphicsDevice->bindTexture(material->specTexture.get(), TBP_Specular);
		}

		for(auto& [bindingPoint, data] : material->shaderData){
			if(auto iter = shaderData.find(bindingPoint); iter == shaderData.end()){ //If we don't have data for that bindingPint
				global::graphicsDevice->bindShaderResourceBuffer(*data.buffer, data.shaderType, bindingPoint);
			}
		}

		for(auto& [bindingPoint, data] : shaderData){
			global::graphicsDevice->bindShaderResourceBuffer(*data.buffer, data.shaderType, bindingPoint);
		}
	}

	void MaterialInstance::setAlbedoTexture(const std::string& path){
		TextureDescriptor tdesc{};
		albedoTexture = global::graphicsFactory->createTexture(tdesc, path);
	}

	void MaterialInstance::setAlbedoTexture(const std::shared_ptr<Texture>& texture){
		albedoTexture = texture;
	}

	void MaterialInstance::setSpecularTexture(const std::string& path){
		TextureDescriptor tdesc{};
		specTexture = global::graphicsFactory->createTexture(tdesc, path);
	}

	void MaterialInstance::setSpecularTexture(const std::shared_ptr<Texture>& texture){
		specTexture = texture;
	}
}