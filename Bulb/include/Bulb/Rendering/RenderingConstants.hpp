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
        Shader   = 0, /**< Shader specific data. Unlikely to be shared across shaders. */
        Model    = 1,
        View     = 2,
        Material = 3, /**< Diffuse, specular, colour, shininess etc.*/
        Lighting = 4,
    };
}