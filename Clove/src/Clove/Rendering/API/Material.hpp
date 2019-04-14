#pragma once

#include "Clove/Rendering/API/RenderingTypes.hpp"

#include <any>

namespace clv{
	class Shader;
	class Texture;

	class Material{
		//VARIABLES
	private:
		std::unique_ptr<Texture> diffuseTexture;
		std::unique_ptr<Texture> specularTexture;

		std::string diffuseTexturePath;
		std::string specularTexturePath;

		std::unordered_map<std::string, std::any> uniformMap;

		ShaderType shaderType;

		//FUNCTIONS
	public:
		CLV_API Material(ShaderType shaderType = ShaderType::standard);
		CLV_API Material(const std::string& texturePath, ShaderType shaderType = ShaderType::standard);
		CLV_API Material(const Material& other);
		CLV_API Material(Material&& other) noexcept;

		CLV_API ~Material();

		CLV_API void setSpecularTexture(const std::string& path);

		template<typename T>
		void setUniform(const std::string& name, const T& value);

		void bind(Shader& shader);
		void unbind();

		CLV_API inline const ShaderType getShaderType() const;

		CLV_API Material& operator=(const Material& other);
		CLV_API Material& operator=(Material&& other) noexcept;

	private:
		void createMaterial(const std::string& texturePath);

		void createSpecularTexture(const std::string& texturePath);
	};
}

#include "Material.inl"