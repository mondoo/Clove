#include "Membrane/EditorLayer.hpp"

#include "Membrane/MessageHandler.hpp"
#include "Membrane/Messages.hpp"
#include "Membrane/NameComponent.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/CameraComponent.hpp>
#include <Clove/Components/CollisionShapeComponent.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/RigidBodyComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Layers/PhysicsLayer.hpp>
#include <Clove/Maths/MathsHelpers.hpp>
#include <Clove/ModelLoader.hpp>
#include <Clove/Surface.hpp>
#include <msclr/marshal_cppstd.h>

namespace garlic::membrane {
    // clang-format off
    /**
     * @brief 
     */
    private ref class EditorLayerMessageProxy { //TODO: This will move into editor layer - runtime layer does not respond to editor
        //VARIABLES
    private:
        EditorLayer *layer{ nullptr };

        //FUNCTIONS
    public:
        EditorLayerMessageProxy(EditorLayer *layer)
            : layer{ layer } {
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_CreateEntity ^>(this, &EditorLayerMessageProxy::createEntity));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_CreateComponent ^>(this, &EditorLayerMessageProxy::createComponent));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_UpdateRigidBody ^>(this, &EditorLayerMessageProxy::updateRigidBody));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_UpdateTransform ^>(this, &EditorLayerMessageProxy::updateTransform));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_UpdateName ^>(this, &EditorLayerMessageProxy::updateName));


            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_SaveScene ^>(this, &EditorLayerMessageProxy::saveScene));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_LoadScene ^>(this, &EditorLayerMessageProxy::loadScene));
        }

    private:
        void createEntity(Editor_CreateEntity ^ message) {
            layer->createEntity();
        }

        void createComponent(Editor_CreateComponent ^ message){
            layer->createComponent(message->entity, message->componentType);
        }

        void updateRigidBody(Editor_UpdateRigidBody^ message){
            layer->updateRigidBody(message->entity, message->mass);
        }

        void updateTransform(Editor_UpdateTransform ^ message){
            clove::vec3f pos{message->position.x, message->position.y, message->position.z};
            clove::vec3f rot{clove::asRadians(message->rotation.x), clove::asRadians(message->rotation.y), clove::asRadians(message->rotation.z)};
            clove::vec3f scale{message->scale.x, message->scale.y, message->scale.z};

            layer->updateTransform(message->entity, pos, rot, scale);
        }

        void updateName(Editor_UpdateName ^ message){
            System::String ^name{ message->name };
            layer->updateName(message->entity, msclr::interop::marshal_as<std::string>(name));
        }

        void saveScene(Editor_SaveScene ^message){
            layer->saveScene();
        }

        void loadScene(Editor_LoadScene ^message){
            layer->loadScene();
        }
    };
    // clang-format on
}

namespace garlic::membrane {
    EditorLayer::EditorLayer()
        : clove::Layer{ "Editor Layer" }
        , currentScene{ clove::Application::get().getEntityManager(), "scene.yaml" } {
        proxy = gcnew EditorLayerMessageProxy(this);
    }

    void EditorLayer::onAttach() {
        auto &app{ clove::Application::get() };

        //Pop the physics layer from the application
        app.popLayer(app.getPhysicsLayer());

        //Add the editor camera outside of the current scene
        if(editorCamera == clove::NullEntity) {
            auto *const entityManager{ app.getEntityManager() };

            editorCamera                                                                  = entityManager->create();
            entityManager->addComponent<clove::TransformComponent>(editorCamera).position = clove::vec3f{ 0.0f, 0.0f, -10.0f };
            entityManager->addComponent<clove::CameraComponent>(editorCamera, clove::Camera{ clove::Camera::ProjectionMode::Perspective });
        }

        loadScene();
    }

    clove::InputResponse EditorLayer::onInputEvent(clove::InputEvent const &inputEvent) {
        if(inputEvent.eventType == clove::InputEvent::Type::Mouse) {
            auto event{ std::get<clove::Mouse::Event>(inputEvent.event) };

            if(event.getButton() == clove::MouseButton::Right) {
                if(event.getType() == clove::Mouse::Event::Type::Pressed) {
                    //Make sure prev + pos gets updated when the event happens.
                    //This fixes situations where the mouse will be up, move and then go down
                    mousePos     = event.getPos();
                    prevMousePos = event.getPos();

                    moveMouse = true;
                    return clove::InputResponse::Consumed;
                } else {
                    moveMouse = false;
                    return clove::InputResponse::Consumed;
                }
            }

            if(event.getType() == clove::Mouse::Event::Type::Move && moveMouse) {
                mousePos = event.getPos();
                return clove::InputResponse::Consumed;
            }
        }
        return clove::InputResponse::Ignored;
    }

    void EditorLayer::onUpdate(clove::DeltaTime const deltaTime) {
        //Broadcast up any transform changes
        for(auto &entity : currentScene.getKnownEntities()) {
            if(currentScene.hasComponent<clove::TransformComponent>(entity)) {
                auto const &pos{ currentScene.getComponent<clove::TransformComponent>(entity).position };
                auto const &rot{ clove::quaternionToEuler(currentScene.getComponent<clove::TransformComponent>(entity).rotation) };
                auto const &scale{ currentScene.getComponent<clove::TransformComponent>(entity).scale };

                Engine_OnTransformChanged ^ message { gcnew Engine_OnTransformChanged };
                message->entity   = entity;
                message->position = Vector3(pos.x, pos.y, pos.z);
                message->rotation = Vector3(clove::asDegrees(rot.x), clove::asDegrees(rot.y), clove::asDegrees(rot.z));
                message->scale    = Vector3(scale.x, scale.y, scale.z);
                MessageHandler::sendMessage(message);
            }
        }

        auto &keyBoard{ clove::Application::get().getSurface()->getKeyboard() };
        auto &mouse{ clove::Application::get().getSurface()->getMouse() };
        auto *const entityManager{ clove::Application::get().getEntityManager() };

        clove::vec3f pos{};
        clove::vec3f rot{};

        //Camera Movement: Keyboard
        if(keyBoard.isKeyPressed(clove::Key::W)) {
            pos.z += 1.0f;
        }
        if(keyBoard.isKeyPressed(clove::Key::S)) {
            pos.z -= 1.0f;
        }
        if(keyBoard.isKeyPressed(clove::Key::A)) {
            pos.x -= 1.0f;
        }
        if(keyBoard.isKeyPressed(clove::Key::D)) {
            pos.x += 1.0f;
        }
        if(keyBoard.isKeyPressed(clove::Key::Space) || keyBoard.isKeyPressed(clove::Key::E)) {
            pos.y += 1.0f;
        }
        if(keyBoard.isKeyPressed(clove::Key::Shift_Left) || keyBoard.isKeyPressed(clove::Key::Q)) {
            pos.y -= 1.0f;
        }

        //Camera Movement: Mouse
        if(moveMouse) {
            float constexpr speed = 10.0f;
            clove::vec2i const delta{ mousePos - prevMousePos };

            mouseLookYaw += static_cast<float>(delta.x) * speed * deltaTime;
            mouseLookPitch += static_cast<float>(delta.y) * speed * deltaTime;

            prevMousePos = mousePos;
        }

        rot.x = clove::asRadians(mouseLookPitch);
        rot.y = clove::asRadians(mouseLookYaw);

        auto &camTrans{ entityManager->getComponent<clove::TransformComponent>(editorCamera) };
        camTrans.rotation = rot;
        camTrans.position += camTrans.rotation * pos * 10.0f * deltaTime.getDeltaSeconds();
    }

    void EditorLayer::onDetach() {
        saveScene();
        currentScene.destroyAllEntities();
    }

    void EditorLayer::saveScene() {
        currentScene.save();
    }

    void EditorLayer::loadScene() {
        currentScene.load();

        Engine_OnSceneLoaded ^ loadMessage { gcnew Engine_OnSceneLoaded };
        loadMessage->entities = gcnew System::Collections::Generic::List<Entity ^>{};
        for(auto entity : currentScene.getKnownEntities()) {
            Entity ^ editorEntity { gcnew Entity };
            editorEntity->id         = entity;
            editorEntity->name       = gcnew System::String(currentScene.getComponent<NameComponent>(entity).name.c_str());
            editorEntity->components = gcnew System::Collections::Generic::List<ComponentType>{};

            //Add all of the component types for an entity
            if(currentScene.hasComponent<clove::TransformComponent>(entity)) {
                editorEntity->components->Add(ComponentType::Transform);
            }
            if(currentScene.hasComponent<clove::StaticModelComponent>(entity)) {
                editorEntity->components->Add(ComponentType::Mesh);
            }
            if(currentScene.hasComponent<clove::PointLightComponent>(entity)) {
                editorEntity->components->Add(ComponentType::PointLight);
            }

            loadMessage->entities->Add(editorEntity);
        }

        MessageHandler::sendMessage(loadMessage);
    }

    clove::Entity EditorLayer::createEntity(std::string_view name) {
        clove::Entity entity{ currentScene.createEntity() };
        currentScene.addComponent<NameComponent>(entity, std::string{ std::move(name) });

        Engine_OnEntityCreated ^ message { gcnew Engine_OnEntityCreated };
        message->entity = entity;
        message->name   = gcnew System::String(std::string{ std::begin(name), std::end(name) }.c_str());
        MessageHandler::sendMessage(message);

        return entity;
    }

    void EditorLayer::createComponent(clove::Entity entity, ComponentType componentType) {
        bool added{ false };
        switch(componentType) {
            case ComponentType::Transform:
                if(!currentScene.hasComponent<clove::TransformComponent>(entity)) {
                    currentScene.addComponent<clove::TransformComponent>(entity);
                    added = true;
                }
                break;
            case ComponentType::Mesh:
                if(!currentScene.hasComponent<clove::StaticModelComponent>(entity)) {
                    currentScene.addComponent<clove::StaticModelComponent>(entity, clove::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj"));//TEMP: Hard coding to a cube for now
                    added = true;
                }
                break;
            case ComponentType::PointLight:
                if(!currentScene.hasComponent<clove::PointLightComponent>(entity)) {
                    currentScene.addComponent<clove::PointLightComponent>(entity);
                    added = true;
                }
                break;
            case ComponentType::RigidBody:
                if(!currentScene.hasComponent<clove::RigidBodyComponent>(entity)) {
                    currentScene.addComponent<clove::RigidBodyComponent>(entity);
                    added = true;
                }
                break;
            case ComponentType::CollisionShape:
                if(!currentScene.hasComponent<clove::CollisionShapeComponent>(entity)) {
                    currentScene.addComponent<clove::CollisionShapeComponent>(entity, clove::CollisionShapeComponent::Cube{});//TEMP: Use basic cube shape
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

    void EditorLayer::updateTransform(clove::Entity entity, clove::vec3f position, clove::vec3f rotation, clove::vec3f scale) {
        if(currentScene.hasComponent<clove::TransformComponent>(entity)) {
            auto &transform{ currentScene.getComponent<clove::TransformComponent>(entity) };
            transform.position = position;
            transform.rotation = rotation;
            transform.scale    = scale;
        }
    }

    void EditorLayer::updateRigidBody(clove::Entity entity, float mass) {
        if(currentScene.hasComponent<clove::RigidBodyComponent>(entity)) {
            auto &rigidBody{ currentScene.getComponent<clove::RigidBodyComponent>(entity) };
            rigidBody.mass = mass;

            //Pipe back up the change to the editor
            Engine_OnRigidBodyChanged ^ message { gcnew Engine_OnRigidBodyChanged };
            message->entity = entity;
            message->mass   = mass;
            MessageHandler::sendMessage(message);
        }
    }

    void EditorLayer::updateName(clove::Entity entity, std::string name) {
        if(currentScene.hasComponent<NameComponent>(entity)) {
            currentScene.getComponent<NameComponent>(entity).name = std::move(name);
        }
    }
}