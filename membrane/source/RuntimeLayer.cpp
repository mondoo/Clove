#include "Membrane/RuntimeLayer.hpp"

#include "Membrane/Messages.hpp"

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

            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_SaveScene ^>(this, &RuntimeLayerMessageProxy::saveScene));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_LoadScene ^>(this, &RuntimeLayerMessageProxy::loadScene));
        }

    private:
        void createEntity(Editor_CreateEntity ^ message) {
            layer->createEntity();
        }

        void createComponent(Editor_CreateComponent ^ message){
            layer->createComponent(message->entity, message->componentType);
        }

        void updateTransform(Editor_UpdateTransform ^ message){
            clove::vec3f pos{message->position.x, message->position.y, message->position.z};
            clove::vec3f rot{clove::asRadians(message->rotation.x), clove::asRadians(message->rotation.y), clove::asRadians(message->rotation.z)};
            clove::vec3f scale{message->scale.x, message->scale.y, message->scale.z};

            layer->updateTransform(message->entity, pos, rot, scale);
        }

        void saveScene(Editor_SaveScene ^message){
            CLOVE_LOG(LOG_CATEGORY_CLOVE, clove::LogLevel::Warning, "Saving not yet handled!");
        }

        void loadScene(Editor_LoadScene ^message){
            CLOVE_LOG(LOG_CATEGORY_CLOVE, clove::LogLevel::Warning, "Loading not yet handled!");
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
        auto lightEnt{ createEntity("Point Light") };
        createComponent(lightEnt, ComponentType::Transform);
        createComponent(lightEnt, ComponentType::PointLight);

        entityManager->getComponent<clove::TransformComponent>(lightEnt).position = clove::vec3f{ 5.0f, 0.0f, 0.0f };

        runtimeEntities.push_back(lightEnt);
    }

    void RuntimeLayer::onUpdate(clove::DeltaTime const deltaTime) {
        static float time{ 0.0f };
        time += deltaTime;

        for(auto &entity : runtimeEntities) {
            if(entityManager->hasComponent<clove::TransformComponent>(entity)) {
                auto const &pos{ entityManager->getComponent<clove::TransformComponent>(entity).position };
                auto const &rot{ clove::quaternionToEuler(entityManager->getComponent<clove::TransformComponent>(entity).rotation) };
                auto const &scale{ entityManager->getComponent<clove::TransformComponent>(entity).scale };

                Engine_OnTransformChanged ^ message { gcnew Engine_OnTransformChanged };
                message->entity   = entity;
                message->position = Vector3(pos.x, pos.y, pos.z);
                message->rotation = Vector3(clove::asDegrees(rot.x), clove::asDegrees(rot.y), clove::asDegrees(rot.z));
                message->scale    = Vector3(scale.x, scale.y, scale.z);
                MessageHandler::sendMessage(message);
            }
        }
    }

    void RuntimeLayer::onDetach() {
        //TODO: Handled by the scene?
        /*for(auto &entity : runtimeEntities) {
            entityManager->destroy(entity);
        }*/
    }

    clove::Entity RuntimeLayer::createEntity(std::string_view name) {
        clove::Entity entity{ entityManager->create() };
        runtimeEntities.push_back(entity);

        Engine_OnEntityCreated ^ message { gcnew Engine_OnEntityCreated };
        message->entity = entity;
        message->name   = gcnew System::String(std::string{ std::begin(name), std::end(name) }.c_str());
        MessageHandler::sendMessage(message);

        return entity;
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
            case ComponentType::PointLight:
                if(!entityManager->hasComponent<clove::PointLightComponent>(entity)) {
                    entityManager->addComponent<clove::PointLightComponent>(entity);
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

    void RuntimeLayer::updateTransform(clove::Entity entity, clove::vec3f position, clove::vec3f rotation, clove::vec3f scale) {
        if(entityManager->hasComponent<clove::TransformComponent>(entity)) {
            entityManager->getComponent<clove::TransformComponent>(entity).position = position;
            entityManager->getComponent<clove::TransformComponent>(entity).rotation = rotation;
            entityManager->getComponent<clove::TransformComponent>(entity).scale = scale;
        }
    }
}