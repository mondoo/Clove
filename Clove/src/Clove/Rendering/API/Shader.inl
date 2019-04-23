#include "Clove/Maths/MathsHelpers.hpp"

namespace clv{
	template<typename T>
	void Shader::setUniform(const std::string& name, const T& value){
		CLV_ASSERT(false, "Uniform type not supported!");
	}
}