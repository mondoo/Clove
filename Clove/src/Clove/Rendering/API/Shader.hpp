#pragma once

namespace clv{
	enum class ShaderTypes{
		Vertex,
		Fragment,
	};

	class Shader{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		std::unordered_map<std::string, int> uniformLocationCache;

		//FUNCTIONS
	public:
		Shader();
		Shader(Shader&& other) noexcept;

		~Shader();

		void bind() const;
		void unbind() const;

		void attachShader(ShaderTypes shaderType, const std::string& path);

		template<typename T>
		void setUniform(const std::string& name, const T& value);

		void setUniform(const std::string& name, const int& value);
		void setUniform(const std::string& name, const float& value);
		void setUniform(const std::string& name, const math::Vector3f& value);
		void setUniform(const std::string& name, const math::Vector4f& value);
		void setUniform(const std::string& name, const math::Matrix4f& value);

		Shader& operator=(Shader&& other) noexcept;

	private:
		std::string parseShader(const std::string& filepath);
		unsigned int createShader();
		unsigned int compileShader(unsigned int type, const std::string& source);

		int getUniformLocation(const std::string& name);
	};
}

#include "Shader.inl"