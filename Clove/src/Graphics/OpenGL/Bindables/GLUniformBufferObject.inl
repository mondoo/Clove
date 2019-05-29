#include <glad/glad.h>

namespace clv::gfx{
	template<typename T>
	inline UniformBufferObject<T>::~UniformBufferObject(){
		glDeleteBuffers(1, &uboID);
	}

	template<typename T>
	inline UniformBufferObject<T>::UniformBufferObject(unsigned int bindingPoint){
		glGenBuffers(1, &uboID);

		glBindBuffer(GL_UNIFORM_BUFFER, uboID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, uboID, 0, sizeof(T));
	}

	template<typename T>
	inline void UniformBufferObject<T>::bind(Renderer& renderer){
	}

	template<typename T>
	inline void UniformBufferObject<T>::unbind(){
	}

	template<typename T>
	inline void UniformBufferObject<T>::update(const T& data, Renderer& renderer){
		glBindBuffer(GL_UNIFORM_BUFFER, uboID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}
