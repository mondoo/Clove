#pragma once

#include "Clove/Layer.hpp"

namespace garlic::clove {
    class EntityManager;
}

namespace garlic::clove {
    /**
	 * @brief Plays poisitional audio
	 */
    class AudioLayer : public Layer{
        //VARIABLES
    private:
        EntityManager *entityManager{ nullptr };
        
        //FUNCTIONS
    public:
        AudioLayer();

        AudioLayer(AudioLayer const &other) = delete;
        AudioLayer(AudioLayer &&other) noexcept;

        AudioLayer &operator=(AudioLayer const &other) = delete;
        AudioLayer &operator=(AudioLayer &&other) noexcept;

        ~AudioLayer();

        void onUpdate(DeltaTime const deltaTime) override;
    };
}
