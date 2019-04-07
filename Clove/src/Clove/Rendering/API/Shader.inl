#include "Clove/Maths/MathsHelpers.hpp"

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
	void Shader::setUniform(const std::string& name, const math::Vector3f& value){
		GLCall(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
	}

	template<>
	void Shader::setUniform(const std::string& name, const math::Vector4f& value){
		GLCall(glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	template<>
	void Shader::setUniform(const std::string& name, const math::Matrix4f& value){
		GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, math::valuePtr(value)));
	}
}