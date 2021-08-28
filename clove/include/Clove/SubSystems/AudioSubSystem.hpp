#pragma once

#include "Clove/SubSystem.hpp"

namespace clove {
    class EntityManager;
}

namespace clove {
    /**
	 * @brief Plays poisitional audio
	 */
    class AudioSubSystem : public SubSystem{
        //VARIABLES
    private:
        EntityManager *entityManager{ nullptr };
        
        //FUNCTIONS
    public:
		AudioSubSystem() = delete;
        AudioSubSystem(EntityManager *entityManager);

        AudioSubSystem(AudioSubSystem const &other) = delete;
        AudioSubSystem(AudioSubSystem &&other) noexcept;

        AudioSubSystem &operator=(AudioSubSystem const &other) = delete;
        AudioSubSystem &operator=(AudioSubSystem &&other) noexcept;

        ~AudioSubSystem();

        void onAttach() override{};
        InputResponse onInputEvent(InputEvent const &inputEvent) override{ return InputResponse::Ignored; }
        void onUpdate(DeltaTime const deltaTime) override;
        void onDetach() override{};
    };
}
