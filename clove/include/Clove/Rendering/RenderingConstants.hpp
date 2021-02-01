#pragma once

#include "Clove/Rendering/AnimationTypes.hpp"

#include <cstdint>
#include <limits>

namespace garlic::clove {
	//Variables present in Constants.glsl
    inline constexpr uint8_t MAX_LIGHTS{ 10u };
    inline constexpr uint8_t MAX_JOINTS{ std::numeric_limits<JointIndexType>::max() };

    //General constants
    inline constexpr uint32_t shadowMapSize{ 1024u };
}