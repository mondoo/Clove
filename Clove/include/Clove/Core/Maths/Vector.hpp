#pragma once

//Wrapping glm in clv::mth

#include "Clove/Core/Maths/glmWrappers.hpp"

#include <glm/glm.hpp>

namespace clv::mth{
	template<length_t L, typename T, qualifier Q>
	using vec = glm::vec<L, T, Q>;

	using vec1i = vec<1, int32_t, qualifier::defaultp>;
	using vec2i = vec<2, int32_t, qualifier::defaultp>;
	using vec3i = vec<3, int32_t, qualifier::defaultp>;
	using vec4i = vec<4, int32_t, qualifier::defaultp>;

	using vec1ui = vec<1, uint32_t, qualifier::defaultp>;
	using vec2ui = vec<2, uint32_t, qualifier::defaultp>;
	using vec3ui = vec<3, uint32_t, qualifier::defaultp>;
	using vec4ui = vec<4, uint32_t, qualifier::defaultp>;

	using vec1f = vec<1, float, qualifier::defaultp>;
	using vec2f = vec<2, float, qualifier::defaultp>;
	using vec3f = vec<3, float, qualifier::defaultp>;
	using vec4f = vec<4, float, qualifier::defaultp>;

	//TODO: All the other ps
}