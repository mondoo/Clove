#pragma once

//Wrapping glm in clv::math

#include "Core/Maths/glmWrappers.hpp"

#include <glm/glm.hpp>

namespace clv::math{
	template<typename T, qualifier Q>
	using Quaternion = glm::qua<T, Q>;

	using Quaternionf = Quaternion<float, qualifier::defaultp>;

	//TODO: All the other ps
}