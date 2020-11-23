#pragma once

#include "Clove/Maths/glmWrappers.hpp"

#include <glm/glm.hpp>

namespace clv::mth {
    template<typename T, qualifier Q>
    using quat = glm::qua<T, Q>;

    using quatf = quat<float, qualifier::defaultp>;
}