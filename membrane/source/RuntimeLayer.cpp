#include "Membrane/RuntimeLayer.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/ModelLoader.hpp>

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
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_CreateComponent ^>(this, &RuntimeLayerMessageProxy::createComponent));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_UpdateTransform ^>(this, &RuntimeLayerMessageProxy::updateTransform));
        }

    private:
        void createEntity(Editor_CreateEntity ^ message) {
            layer->createEntity();
        }

        void createComponent(Editor_CreateComponent ^ message){
            layer->createComponent(message->entity, message->componentType);
        }

        void updateTransform(Editor_UpdateTransform ^ message){
            layer->updateTransform(message->entity, clove::vec3f{ message->position.x, message->position.y, message->position.z });
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

    void RuntimeLayer::onUpdate(clove::DeltaTime const deltaTime) {
        entityManager->forEach([](clove::TransformComponent const &transform) {
            auto const &pos{ transform.getPosition() };

            Engine_OnTransformChanged ^ message { gcnew Engine_OnTransformChanged };
            message->entity   = transform.getEntity();
            message->position = Vector3(pos.x, pos.y, pos.z);
        });
    }

    void RuntimeLayer::onDetach() {
        for(auto &entity : runtimeEntities) {
            entityManager->destroy(entity);
        }
    }

    void RuntimeLayer::createEntity() {
        clove::Entity entity{ entityManager->create() };
        runtimeEntities.push_back(entity);

        Engine_OnEntityCreated ^ message { gcnew Engine_OnEntityCreated };
        message->entity = entity;
        MessageHandler::sendMessage(message);
    }

    void RuntimeLayer::createComponent(clove::Entity entity, ComponentType componentType) {
        bool added{ false };
        switch(componentType) {
            case ComponentType::Transform:
                if(!entityManager->hasComponent<clove::TransformComponent>(entity)) {
                    entityManager->addComponent<clove::TransformComponent>(entity);
                    added = true;
                }
                break;
            case ComponentType::Mesh:
                if(!entityManager->hasComponent<clove::StaticModelComponent>(entity)) {
                    entityManager->addComponent<clove::StaticModelComponent>(entity, clove::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj"));
                    added = true;
                }
                break;
        }

        if(added) {
            Engine_OnComponentCreated ^ message { gcnew Engine_OnComponentCreated };
            message->entity        = entity;
            message->componentType = componentType;
            MessageHandler::sendMessage(message);
        }
    }

    void RuntimeLayer::updateTransform(clove::Entity entity, clove::vec3f position) {
        if(entityManager->hasComponent<clove::TransformComponent>(entity)) {
            entityManager->getComponent<clove::TransformComponent>(entity)->setPosition(position);
        }
    }
}