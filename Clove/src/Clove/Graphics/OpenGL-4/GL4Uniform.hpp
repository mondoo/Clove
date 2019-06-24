#pragma once

namespace clv::gfx{
	template<typename T>
	class GL4Uniform{
		//VARIABLES
	private:
		std::string uniformName;
		T uniformValue;

		unsigned int cachedProgramID = 0;
		unsigned int cachedUniformLocation = 0;

		//FUNCTIONS
	public:
		GL4Uniform() = delete;
		GL4Uniform(const GL4Uniform& other) = delete;
		GL4Uniform(GL4Uniform&& other) noexcept = default;
		GL4Uniform& operator=(const GL4Uniform& other) = delete;
		GL4Uniform& operator=(GL4Uniform&& other) noexcept = default;
		virtual ~GL4Uniform() = default;

		GL4Uniform(const std::string& uniformName);
		GL4Uniform(const std::string& uniformName, const T& uniformValue);

		void bind(unsigned int programID);
		void update(const T& newValue);

	private:
		void initialiseUniformLocation(unsigned int programID);

		void applyValue();
	};
}

#include "GL4Uniform.inl"