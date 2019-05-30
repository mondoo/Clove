#include "Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	template<typename T>
	inline GLUniform<T>::GLUniform(const std::string& uniformName)
		: uniformName(uniformName){
	}

	template<typename T>
	inline GLUniform<T>::GLUniform(const std::string& uniformName, const T& uniformValue)
		: uniformName(uniformName)
		, uniformValue(uniformValue){
	}

	template<typename T>
	inline void GLUniform<T>::bind(unsigned int programID){
		if(cachedProgramID != programID){
			initialiseUniformLocation(programID);
		}

		applyValue();
	}

	template<typename T>
	inline void GLUniform<T>::update(const T& newValue){
		uniformValue = newValue;
	}

	template<typename T>
	inline void GLUniform<T>::initialiseUniformLocation(unsigned int programID){
		int location = glGetUniformLocation(programID, uniformName.c_str());
		if(location == -1){
			CLV_LOG_WARN("Warning: Uniform {0} does not exist!: {1}", uniformName, __FUNCTION__);
		}
		cachedUniformLocation = location;
	}

	template<typename T>
	inline void GLUniform<T>::applyValue(){
		CLV_ASSERT(false, "Unsupported type!");
	}

	inline void GLUniform<int>::applyValue(){
		glUniform1i(cachedUniformLocation, uniformValue);
	}

	inline void GLUniform<float>::applyValue(){
		glUniform1f(cachedUniformLocation, uniformValue);
	}

	inline void GLUniform<math::Vector2f>::applyValue(){
		glUniform2fv(cachedUniformLocation, GL_FALSE, math::valuePtr(uniformValue));
	}

	inline void GLUniform<math::Vector3f>::applyValue(){
		glUniform3fv(cachedUniformLocation, GL_FALSE, math::valuePtr(uniformValue));
	}

	inline void GLUniform<math::Matrix4f>::applyValue(){
		glUniformMatrix4fv(cachedUniformLocation, 1, GL_FALSE, math::valuePtr(uniformValue));
	}
}