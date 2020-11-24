#pragma once

#include "Clove/Maths/glmWrappers.hpp"

#include <glm/detail/type_quat.hpp>

namespace garlic::clove {
    template<typename T, qualifier Q>
    using quat = glm::qua<T, Q>;

    using quatf = quat<float, qualifier::defaultp>;
}