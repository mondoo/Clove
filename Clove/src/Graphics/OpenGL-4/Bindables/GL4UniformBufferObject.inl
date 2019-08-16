#include <glad/glad.h>

namespace clv::gfx{
	template<typename T>
	GL4UniformBufferObject<T>::~GL4UniformBufferObject(){
		glDeleteBuffers(1, &uboID);
	}

	template<typename T>
	GL4UniformBufferObject<T>::GL4UniformBufferObject(uint32 bindingPoint)
		: bindingPoint(bindingPoint){
		glGenBuffers(1, &uboID);

		glBindBuffer(GL_UNIFORM_BUFFER, uboID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboID);
	}

	template<typename T>
	GL4UniformBufferObject<T>::GL4UniformBufferObject(uint32 bindingPoint, const T& data){
		glGenBuffers(1, &uboID);

		glBindBuffer(GL_UNIFORM_BUFFER, uboID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboID);
	}

	template<typename T>
	void GL4UniformBufferObject<T>::bind(){
		glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	}

	template<typename T>
	void GL4UniformBufferObject<T>::update(const T& data){
		glBindBuffer(GL_UNIFORM_BUFFER, uboID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}
