#pragma once

#include "Clove/Maths/glmWrappers.hpp"

#include <glm/glm.hpp>

namespace garlic::clove {
    template<typename T, qualifier Q>
    using quat = glm::qua<T, Q>;

    using quatf = quat<float, qualifier::defaultp>;
}