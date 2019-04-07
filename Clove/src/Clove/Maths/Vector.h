#pragma once

//Wrapping glm in clv::math

#include "Clove/Maths/glmWrappers.h"

#include <glm/glm.hpp>

namespace clv{
	namespace math{
		template<length_t L, typename T, qualifier Q>
		using Vector = glm::vec<L, T, Q>;
		
		using Vector1f = Vector<1, float, qualifier::defaultp>;
		using Vector2f = Vector<2, float, qualifier::defaultp>;
		using Vector3f = Vector<3, float, qualifier::defaultp>;
		using Vector4f = Vector<4, float, qualifier::defaultp>;

		//TODO: All the other ps
	}
}