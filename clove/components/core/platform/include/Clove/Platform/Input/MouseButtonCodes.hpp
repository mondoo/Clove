#pragma once

#include <ostream>

namespace clove {
    enum class MouseButton {
        None,
        Undefined,

        _1,
        _2,
        _3,
        _4,
        _5,

        Left   = _1,
        Right  = _2,
        Middle = _3,
    };

    inline std::ostream &operator<<(std::ostream &os, MouseButton button) {
        return os << static_cast<int32_t>(button);
    }
}
