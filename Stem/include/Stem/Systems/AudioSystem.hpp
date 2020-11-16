#pragma once

#include <Bulb/ECS/System.hpp>

namespace garlic::inline stem {
    /**
	 * @brief Enables an ecs::World to play poisitional audio
	 */
    class AudioSystem : public blb::ecs::System {
        //FUNCTIONS
    public:
        AudioSystem();

        AudioSystem(AudioSystem const &other) = delete;
        AudioSystem(AudioSystem &&other) noexcept;

        AudioSystem &operator=(AudioSystem const &other) = delete;
        AudioSystem &operator=(AudioSystem &&other) noexcept;

        ~AudioSystem();

        void update(blb::ecs::World &world, clv::utl::DeltaTime deltaTime) override;
    };
}
