#pragma once

namespace clv::gfx{
	template<typename T>
	class GLUniform{
		//VARIABLES
	private:
		std::string uniformName;
		T uniformValue;

		unsigned int cachedProgramID = 0;
		unsigned int cachedUniformLocation = 0;

		//FUNCTIONS
	public:
		GLUniform() = delete;
		GLUniform(const GLUniform& other) = delete;
		GLUniform(GLUniform&& other) noexcept = default;
		GLUniform& operator=(const GLUniform& other) = delete;
		GLUniform& operator=(GLUniform&& other) noexcept = default;
		virtual ~GLUniform() = default;

		GLUniform(const std::string& uniformName);
		GLUniform(const std::string& uniformName, const T& uniformValue);

		void bind(unsigned int programID);
		void update(const T& newValue);

	private:
		void initialiseUniformLocation(unsigned int programID);

		void applyValue();
	};
}

#include "GLUniform.inl"