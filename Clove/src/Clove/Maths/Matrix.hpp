#pragma once

//Wrapping glm in clv::math

#include "Clove/Maths/glmWrappers.hpp"

#include <glm/glm.hpp>

namespace clv::math{
	template<length_t C, length_t R, typename T, qualifier Q>
	using Matrix = glm::mat<C, R, T, Q>;

	using Matrix2x2f	= Matrix<2, 2, float, qualifier::defaultp>;
	using Matrix2x3f	= Matrix<2, 3, float, qualifier::defaultp>;
	using Matrix2x4f	= Matrix<2, 4, float, qualifier::defaultp>;
	using Matrix2f		= Matrix2x2f;

	using Matrix3x2f	= Matrix<3, 2, float, qualifier::defaultp>;
	using Matrix3x3f	= Matrix<3, 3, float, qualifier::defaultp>;
	using Matrix3x4f	= Matrix<3, 4, float, qualifier::defaultp>;
	using Matrix3f		= Matrix3x3f;

	using Matrix4x2f	= Matrix<4, 2, float, qualifier::defaultp>;
	using Matrix4x3f	= Matrix<4, 3, float, qualifier::defaultp>;
	using Matrix4x4f	= Matrix<4, 4, float, qualifier::defaultp>;
	using Matrix4f		= Matrix4x4f;

	//TODO: All the other ps
}