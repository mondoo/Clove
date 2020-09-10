#pragma once

namespace blb::rnd {
    /**
	 * All variables here are reflected in Constants.glsl.
	 */

    inline constexpr uint8_t MAX_LIGHTS     = 10u;
    inline constexpr uint32_t shadowMapSize = 1024u;

    inline constexpr uint8_t MAX_JOINTS = std::numeric_limits<uint8_t>::max();

    /**
	 * @brief Defines a mapping between set slots
	 */
    enum class DescriptorSetSlots {
        PerPimitive = 0, /**< For data that will change per primitive submitted. */
        View        = 1,
        Lighting    = 2,

        Lighting = 4,
    };
}