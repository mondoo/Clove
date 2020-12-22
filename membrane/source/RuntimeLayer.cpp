#include "Membrane/RuntimeLayer.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>

namespace garlic::membrane {
    // clang-format off
    /**
     * @brief 
     */
    private ref class RuntimeLayerMessageProxy {
        //VARIABLES
    private:
        class RuntimeLayer *layer{ nullptr };

        //FUNCTIONS
    public:
        RuntimeLayerMessageProxy(class RuntimeLayer *layer)
            : layer{ layer } {
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_CreateEntity ^>(this, &RuntimeLayerMessageProxy::createEntity));
        }

    private:
        void createEntity(Editor_CreateEntity ^ message) {
            layer->createEntity();
        }
    };
    // clang-format on
}

namespace garlic::membrane {
    RuntimeLayer::RuntimeLayer()
        : clove::Layer{ "Runtime Layer" }
        , entityManager{ clove::Application::get().getEntityManager() } {
        proxy = gcnew RuntimeLayerMessageProxy(this);
    }

    void RuntimeLayer::onAttach() {
        auto lightEnt{ entityManager->create() };
        entityManager->addComponent<clove::TransformComponent>(lightEnt)->setPosition({ 5.0f, 0.0f, 0.0f });
        entityManager->addComponent<clove::PointLightComponent>(lightEnt);

        runtimeEntities.push_back(lightEnt);
    }

    void RuntimeLayer::onDetach() {
        for(auto &entity : runtimeEntities) {
            entityManager->destroy(entity);
        }
    }

    void RuntimeLayer::createEntity() {
        clove::Entity entity{ entityManager->create() };

        Engine_OnEntityCreated ^ message { gcnew Engine_OnEntityCreated };
        message->entity = entity;
        MessageHandler::sendMessage(message);
    }
}