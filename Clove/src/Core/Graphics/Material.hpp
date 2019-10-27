#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/Bindables/ShaderBufferObject.hpp"
#include "Core/Graphics/Bindables/Shader.hpp" //TODO: remove? needed for the reflection data

namespace clv::gfx{
	class Shader;
	class Texture;
	class MaterialInstance;
	class Bindable;

	class Material : public std::enable_shared_from_this<Material>{
		friend class MaterialInstance;

		//VARIABLES
	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;
		std::unordered_map<BufferBindingPoint, std::shared_ptr<Bindable>> shaderData;

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

		MaterialInstance createInstance();

		const ShaderReflectionData& getReflectionData() const;
		const std::shared_ptr<Shader>& getShader() const;

		template<typename T>
		void setData(BufferBindingPoint bindingPoint, T&& data, gfx::ShaderType shaderType);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture); 

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}

#include "Material.inl"