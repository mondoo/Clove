#pragma once

#include "Membrane/EditorTypes.hpp"
#include "Membrane/MessageHandler.hpp"

#include <Clove/ECS/Entity.hpp>
#include <Clove/Layer.hpp>
#include <Clove/World.hpp>
#include <msclr/gcroot.h>
#include <string_view>
#include <vector>

namespace garlic::clove {
    class EntityManager;
}

namespace garlic::membrane {
    /**
     * @brief The layer that contains all the run time components.
     * @details This is the layer that will actually hold the entities that
     * make up the game and what will be serialsed when saving the scene.
     */
    class RuntimeLayer : public clove::Layer {
        friend ref class RuntimeLayerMessageProxy;

        //VARIABLES
    private:
        msclr::gcroot<RuntimeLayerMessageProxy ^> proxy;

        clove::EntityManager *entityManager{ nullptr };
        std::vector<clove::Entity> runtimeEntities; //TODO: Remove? Keeping just so update transforms etc.

        //TODO: Move to clove::Application It's likely that worlds will be loaded outside of bulb
        clove::World currentWorld;

        //FUNCTIONS
    public:
        RuntimeLayer();

        void onAttach() override;
        void onUpdate(clove::DeltaTime const deltaTime) override;
        void onDetach() override;

    private:
        clove::Entity createEntity(std::string_view name = "New Entity");
        void createComponent(clove::Entity entity, ComponentType componentType);

        void updateTransform(clove::Entity entity, clove::vec3f position, clove::vec3f rotation, clove::vec3f scale);
    };
}