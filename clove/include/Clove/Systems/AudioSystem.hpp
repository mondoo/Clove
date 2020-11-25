#pragma once

#include <Clove/ECS/System.hpp>

namespace garlic::clove {
    /**
	 * @brief Enables an ecs::World to play poisitional audio
	 */
    class AudioSystem : public System {
        //FUNCTIONS
    public:
        AudioSystem();

        AudioSystem(AudioSystem const &other) = delete;
        AudioSystem(AudioSystem &&other) noexcept;

        AudioSystem &operator=(AudioSystem const &other) = delete;
        AudioSystem &operator=(AudioSystem &&other) noexcept;

        ~AudioSystem();

        void update(World &world, DeltaTime deltaTime) override;
    };
}
