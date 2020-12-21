#pragma once

#include <memory>

namespace garlic::clove {
    class AudioListener;
}

namespace garlic::clove {
    /**
	 * @brief Allows an entity to determine where a sound comes from.
	 * @details Every ecs::EntityManager can only have 1 AudioListenerComponent.
	 */
    struct AudioListenerComponent {
        std::unique_ptr<AudioListener> listener;
    };
}
