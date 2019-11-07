#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/Resources/ShaderResource.hpp"
#include "Core/Graphics/Shader.hpp" //TODO: remove? needed for the reflection data

namespace clv::gfx{
	class Texture;
	class MaterialInstance;
	class Bindable;
}

namespace clv::gfx{
	class Material : public std::enable_shared_from_this<Material>{
		friend class MaterialInstance;

		//VARIABLES
	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;
		std::unordered_map<BufferBindingPoint, std::shared_ptr<ShaderResource>> shaderData;

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

		template<typename DataType>
		void setData(BufferBindingPoint bindingPoint, DataType&& data, ShaderType shaderType);

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}

#include "Material.inl"