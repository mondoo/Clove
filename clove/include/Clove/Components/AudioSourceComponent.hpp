#pragma once

#include <Clove/Audio/AudioSource.hpp>

#include <memory>

namespace garlic::clove {
    /**
	 * @brief Allows an entity to determine where a sound is coming from.
	 */
    struct AudioSourceComponent {
        std::unique_ptr<AudioSource> source;
    };
}
