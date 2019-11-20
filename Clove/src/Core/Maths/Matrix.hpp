#pragma once

//Wrapping glm in clv::mth

#include "Core/Maths/glmWrappers.hpp"

#include <glm/glm.hpp>

namespace clv::mth{
	template<length_t C, length_t R, typename T, qualifier Q>
	using mat = glm::mat<C, R, T, Q>;

	using mat2x2f	= mat<2, 2, float, qualifier::defaultp>;
	using mat2f		= mat2x2f;

	using mat3x3f	= mat<3, 3, float, qualifier::defaultp>;
	using mat3f		= mat3x3f;

	using mat4x4f	= mat<4, 4, float, qualifier::defaultp>;
	using mat4f		= mat4x4f;

	//TODO: All the other ps
}