#include "Clove/Graphics/OpenGL-4/GL4Exception.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	template<typename T>
	inline GL4Uniform<T>::GL4Uniform(const std::string& uniformName)
		: uniformName(uniformName){
	}

	template<typename T>
	inline GL4Uniform<T>::GL4Uniform(const std::string& uniformName, const T& uniformValue)
		: uniformName(uniformName)
		, uniformValue(uniformValue){
	}

	template<typename T>
	inline void GL4Uniform<T>::bind(unsigned int programID){
		if(cachedProgramID != programID){
			initialiseUniformLocation(programID);
		}

		applyValue();
	}

	template<typename T>
	inline void GL4Uniform<T>::update(const T& newValue){
		uniformValue = newValue;
	}

	template<typename T>
	inline void GL4Uniform<T>::initialiseUniformLocation(unsigned int programID){
		int location = glGetUniformLocation(programID, uniformName.c_str());
		if(location == -1){
			CLV_LOG_WARN("Warning: Uniform {0} does not exist!: {1}", uniformName, __FUNCTION__);
		}
		cachedUniformLocation = location;
	}

	template<typename T>
	inline void GL4Uniform<T>::applyValue(){
		CLV_ASSERT(false, "Unsupported type!");
	}

	template<>
	inline void GL4Uniform<int>::applyValue(){
		glUniform1i(cachedUniformLocation, uniformValue);
	}

	template<>
	inline void GL4Uniform<float>::applyValue(){
		glUniform1f(cachedUniformLocation, uniformValue);
	}

	template<>
	inline void GL4Uniform<math::Vector2f>::applyValue(){
		glUniform2fv(cachedUniformLocation, GL_FALSE, math::valuePtr(uniformValue));
	}

	template<>
	inline void GL4Uniform<math::Vector3f>::applyValue(){
		glUniform3fv(cachedUniformLocation, GL_FALSE, math::valuePtr(uniformValue));
	}

	template<>
	inline void GL4Uniform<math::Matrix4f>::applyValue(){
		glUniformMatrix4fv(cachedUniformLocation, 1, GL_FALSE, math::valuePtr(uniformValue));
	}
}