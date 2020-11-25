#pragma once

#include "Clove/Maths/glmWrappers.hpp"

#include <glm/detail/type_quat.hpp>

namespace garlic::clove {
    template<typename T>
    using quat = glm::qua<T, qualifier::defaultp>;

    using quatf = quat<float>;
}