#pragma once

namespace garlic::clove {
    class World;
}

namespace garlic::clove {
    /**
	 * @brief Plays poisitional audio
	 */
    class AudioSystem {
        //FUNCTIONS
    public:
        AudioSystem();

        AudioSystem(AudioSystem const &other) = delete;
        AudioSystem(AudioSystem &&other) noexcept;

        AudioSystem &operator=(AudioSystem const &other) = delete;
        AudioSystem &operator=(AudioSystem &&other) noexcept;

        ~AudioSystem();

        void update(World &world);
    };
}
