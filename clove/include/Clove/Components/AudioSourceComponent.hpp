#pragma once

#include <memory>

namespace garlic::clove {
    class AudioSource;
}

namespace garlic::clove {
    /**
	 * @brief Allows an entity to determine where a sound is coming from.
	 */
    struct AudioSourceComponent {
        std::unique_ptr<AudioSource> source;
    };
}
