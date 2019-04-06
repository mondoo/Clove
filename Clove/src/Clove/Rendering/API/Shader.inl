#include <glm/gtc/type_ptr.hpp>

namespace clv{
	template<typename T>
	void Shader::setUniform(const std::string& name, const T& value){
		CLV_ASSERT(false, "Uniform type not supported!");
	}

	template<>
	void Shader::setUniform(const std::string& name, const int& value){
		GLCall(glUniform1i(getUniformLocation(name), value));
	}

	template<>
	void Shader::setUniform(const std::string& name, const float& value){
		GLCall(glUniform1f(getUniformLocation(name), value));
	}

	template<>
	void Shader::setUniform(const std::string& name, const glm::vec3& value){
		GLCall(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
	}

	template<>
	void Shader::setUniform(const std::string& name, const glm::vec4& value){
		GLCall(glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	template<>
	void Shader::setUniform(const std::string& name, const glm::mat4& value){
		GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
	}
}