#pragma once

//Wrapping glm in clv::mth

#include "Core/Maths/glmWrappers.hpp"

#include <glm/glm.hpp>

namespace clv::mth{
	template<length_t L, typename T, qualifier Q>
	using vec = glm::vec<L, T, Q>;

	using vec1f = vec<1, float, qualifier::defaultp>;
	using vec2f = vec<2, float, qualifier::defaultp>;
	using vec3f = vec<3, float, qualifier::defaultp>;
	using vec4f = vec<4, float, qualifier::defaultp>;

	//TODO: All the other ps
}