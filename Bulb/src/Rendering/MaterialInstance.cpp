#include "Bulb/Rendering/MaterialInstance.hpp"

#include "Bulb/Rendering/Material.hpp"
#include "Clove/Graphics/Texture.hpp"
#include "Clove/Graphics/CommandBuffer.hpp"

using namespace clv::gfx;

namespace blb::rnd{
	MaterialInstance::MaterialInstance(std::shared_ptr<Material> material)
		: material(std::move(material)){
	}

	MaterialInstance::MaterialInstance(const MaterialInstance& other) = default;

	MaterialInstance::MaterialInstance(MaterialInstance&& other) noexcept{
		material		= std::move(other.material);
		albedoTexture	= std::move(other.albedoTexture);
		specTexture		= std::move(other.specTexture);
		shaderData		= std::move(other.shaderData);
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

	void MaterialInstance::setData(clv::gfx::BufferBindingPoint bindingPoint, void* data, const size_t sizeBytes, clv::gfx::ShaderStage shaderType) {
        if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()) {
            iter->second.buffer->updateData(data);
        } else {
            clv::gfx::BufferDescriptor srdesc{};
            srdesc.elementSize = 0;
            srdesc.bufferSize  = sizeBytes;
            srdesc.bufferType  = clv::gfx::BufferType::ShaderResourceBuffer;
            srdesc.bufferUsage = clv::gfx::BufferUsage::Dynamic;

            auto buffer              = material->graphicsFactory->createBuffer(srdesc, data);
            shaderData[bindingPoint] = { buffer, shaderType };
        }
    }

	void MaterialInstance::setAlbedoTexture(const std::string& path){
		TextureDescriptor tdesc{};
		albedoTexture = material->graphicsFactory->createTexture(tdesc, path);
	}

	void MaterialInstance::setAlbedoTexture(std::shared_ptr<Texture> texture){
		albedoTexture = std::move(texture);
	}

	void MaterialInstance::setSpecularTexture(const std::string& path){
		TextureDescriptor tdesc{};
		specTexture = material->graphicsFactory->createTexture(tdesc, path);
	}

	void MaterialInstance::setSpecularTexture(std::shared_ptr<Texture> texture){
		specTexture = std::move(texture);
	}
}