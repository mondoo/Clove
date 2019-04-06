#pragma once

//TODO:: will probably need some sort of material instance

namespace clv{
	class Shader;
	class Texture;

	class Material{
		//VARIABLES
	private:
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;

		std::string texturePath;

		//FUNCTIONS
	public:
		CLV_API Material();
		CLV_API Material(const std::string& texturePath);
		CLV_API Material(const Material& other);
		CLV_API Material(Material&& other);

		CLV_API ~Material();

		void bindShader();
		void unbindShader();

		void bindTexture();
		void unbindTexture();

		inline const Shader& getShaderData() const;

		//TODO: I'd like this abstracted out but that includes the shader header here and thus causing the client to include glad
		/*template<typename T>
		void setUniform(const std::string& name, const T& value);*/
		void setUniform1i(const std::string& name, int value);
		void setUniform1f(const std::string& name, float value);
		void setUniform3f(const std::string& name, const glm::vec3& value);
		void setUniform4f(const std::string& name, const glm::vec4& value);
		void setUniform4m(const std::string& name, const glm::mat4& value);
		//~END

		CLV_API Material& operator=(const Material& other);
		CLV_API Material& operator=(Material&& other);

	private:
		void createMaterial(const std::string& texturePath);
	};
}

#include "Material.inl"