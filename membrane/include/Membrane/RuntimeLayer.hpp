#pragma once

#include <Clove/ECS/Entity.hpp>
#include <Clove/Layer.hpp>
#include <vector>

namespace garlic::membrane {
    /**
     * @brief The layer that contains all the run time components.
     * @details This is the layer that will actually hold the entities that
     * make up the game.
     */
    class RuntimeLayer : public clove::Layer {
        //VARIABLES
    private:
        std::vector<clove::Entity> runtimeEntities;

        //FUNCTIONS
    public:
        RuntimeLayer()
            : clove::Layer("Runtime Layer") {}

        void onAttach() override;
        void onDetach() override;

        void addEntity();
    };
}