#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp" //TODO: remove? needed for the reflection data

namespace clv::gfx{
	class Shader;
	class Texture;
	class MaterialInstance;

	class Material : public std::enable_shared_from_this<Material>{
		//VARIABLES
	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;
		//std::unordered_map<BufferBindingPoint, std::shared_ptr<Bindable>> shaderData;

		ShaderReflectionData reflectionData;

		//FUNCTIONS
	public:
		Material() = delete;
		Material(const Material& other);
		Material& operator=(const Material& other);
		Material(Material&& other) noexcept;
		Material& operator=(Material&& other) noexcept;
		~Material();

		Material(ShaderStyle shaderStyle);

		void bind();

		MaterialInstance createInstance(); //wish it could be const :(

		//TODO: Maybe not have these getters?
		const ShaderReflectionData& getReflectionData() const;//Have a function to get vertex layout?
		const std::shared_ptr<Shader>& getShader() const;

		template<typename T>
		void setData(BufferBindingPoint bindingPoint, T&& data, gfx::ShaderType shaderType);

		/*
		Thinking something like this
		*/
		//template<typename T>
		//void setData(std::string bufferName, T&& data);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture); 

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}

#include "Material.inl"