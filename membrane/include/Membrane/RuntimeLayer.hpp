#pragma once

#include "Membrane/MessageHandler.hpp"

#include <Clove/ECS/Entity.hpp>
#include <Clove/Layer.hpp>
#include <msclr/gcroot.h>
#include <vector>

namespace garlic::clove {
    class EntityManager;
}

namespace garlic::membrane {
    /**
     * @brief The layer that contains all the run time components.
     * @details This is the layer that will actually hold the entities that
     * make up the game.
     */
    class RuntimeLayer : public clove::Layer {
        friend ref class RuntimeLayerMessageProxy;

        //VARIABLES
    private:
        msclr::gcroot<RuntimeLayerMessageProxy ^> proxy;

        clove::EntityManager *entityManager{ nullptr };
        std::vector<clove::Entity> runtimeEntities;

        //FUNCTIONS
    public:
        RuntimeLayer();

        void onAttach() override;
        void onDetach() override;

    private:
        void createEntity();
        void createComponent(clove::Entity entity, ComponentType componentType);
    };
}