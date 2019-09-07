#pragma once

#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp" //TODO: Remove? needed for the shader reflection data

//TODO: Move this into the material file???
namespace clv::gfx{
	class Material;
	class Texture;

	class MaterialInstance{
		//VARIABLES
	private:
		std::shared_ptr<Material> material;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;
		std::unordered_map<BufferBindingPoint, std::shared_ptr<Bindable>> shaderData;

		//FUNCTIONS
	public:
		MaterialInstance() = delete;
		MaterialInstance(const MaterialInstance& other);
		MaterialInstance& operator=(const MaterialInstance& other);
		MaterialInstance(MaterialInstance&& other) noexcept;
		MaterialInstance& operator=(MaterialInstance&& other) noexcept;

		MaterialInstance(std::shared_ptr<Material> material);

		void bind();

		const ShaderReflectionData& getReflectionData() const;
		const std::shared_ptr<Shader>& getShader() const;

		//Temp: straight up wrapper
		//TODO: inl
		template<typename T>
		void setData(BufferBindingPoint bindingPoint, T&& data, gfx::ShaderType shaderType){
			if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()){
				if(auto sbo = std::dynamic_pointer_cast<ShaderBufferObject<T>>(iter->second)){
					sbo->update(std::forward<T>(data));
					return;
				}
			}
			shaderData[bindingPoint] = BindableFactory::createShaderBufferObject<T>(shaderType, bindingPoint, std::forward<T>(data));
		}

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}