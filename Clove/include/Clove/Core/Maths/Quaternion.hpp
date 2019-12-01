#pragma once

//Wrapping glm in clv::mth

#include "Clove/Core/Maths/glmWrappers.hpp"

#include <glm/glm.hpp>

namespace clv::mth{
	template<typename T, qualifier Q>
	using quat = glm::qua<T, Q>;

	using quatf = quat<float, qualifier::defaultp>;

	//TODO: All the other ps
}