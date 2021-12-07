#pragma once

#include "Membrane/Scene.hpp"

#include <Clove/SubSystem.hpp>

namespace clove {
    class EntityManager;
}

namespace membrane {
    /**
     * @brief The sub system that is active while the game is running.
     * Deliberately does not handle editor events to simulate the game running.
     */
    class RuntimeSubSystem : public clove::SubSystem {
        //VARIABLES
    private:
        Scene currentScene;

        //FUNCTIONS
    public:
        RuntimeSubSystem();

        Group getGroup() const override;

        void onAttach() override;
        clove::InputResponse onInputEvent(clove::InputEvent const &inputEvent) override { return clove::InputResponse::Ignored; }
        void onUpdate(clove::DeltaTime const deltaTime) override;
        void onDetach() override;
    };
}