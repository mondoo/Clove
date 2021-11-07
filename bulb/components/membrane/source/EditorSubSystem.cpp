#include "Membrane/EditorSubSystem.hpp"

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
#include <Clove/Log/Log.hpp>
#include <Clove/Maths/MathsHelpers.hpp>
#include <Clove/ModelLoader.hpp>
#include <Clove/Reflection/Reflection.hpp>
#include <Clove/SubSystems/PhysicsSubSystem.hpp>
#include <msclr/marshal_cppstd.h>

namespace membrane {
    // clang-format off
    /**
     * @brief 
     */
    private ref class EditorSubSystemMessageProxy {
        //VARIABLES
    private:
        EditorSubSystem *subSystem{ nullptr };

        //FUNCTIONS
    public:
        EditorSubSystemMessageProxy(EditorSubSystem *layer)
            : subSystem{ layer } {
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_CreateEntity ^>(this, &EditorSubSystemMessageProxy::createEntity));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_DeleteEntity ^>(this, &EditorSubSystemMessageProxy::deleteEntity));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_AddComponent ^>(this, &EditorSubSystemMessageProxy::addComponent));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_RemoveComponent ^>(this, &EditorSubSystemMessageProxy::removeComponent));

            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_SaveScene ^>(this, &EditorSubSystemMessageProxy::saveScene));
            MessageHandler::bindToMessage(gcnew MessageSentHandler<Editor_LoadScene ^>(this, &EditorSubSystemMessageProxy::loadScene));
        }

    public:
        void reset(){
            subSystem = nullptr;
        }

    private:
        void createEntity(Editor_CreateEntity ^ message) {
            if (subSystem){
                subSystem->createEntity();
            }
        }

        void deleteEntity(Editor_DeleteEntity ^ message) {
            if (subSystem){
                clove::Entity const entity{ message->entity };
                subSystem->deleteEntity(entity);
            }
        }

        void addComponent(Editor_AddComponent ^ message){
            if (subSystem){
                System::String ^typeName{ message->componentName };
                subSystem->addComponent(message->entity, msclr::interop::marshal_as<std::string>(typeName));
            }
        }

        void removeComponent(Editor_RemoveComponent ^ message){
            if (subSystem){
                System::String ^typeName{ message->componentName };
                subSystem->removeComponent(message->entity, msclr::interop::marshal_as<std::string>(typeName));
            }
        }

        void updateName(Editor_UpdateName ^ message){
            if (subSystem){
                System::String ^name{ message->name };
                subSystem->updateName(message->entity, msclr::interop::marshal_as<std::string>(name));
            }
        }

        void saveScene(Editor_SaveScene ^message){
            if (subSystem){
                subSystem->saveScene();
            }
        }

        void loadScene(Editor_LoadScene ^message){
            if (subSystem){
                subSystem->loadScene();
            }
        }
    };
    // clang-format on
}

namespace membrane {
    EditorSubSystem::EditorSubSystem()
        : clove::SubSystem{ "Editor SubSystem" }
        , currentScene{ clove::Application::get().getEntityManager() } {
        proxy = gcnew EditorSubSystemMessageProxy(this);
    }

    EditorSubSystem::~EditorSubSystem() {
        proxy->reset();
    }

    void EditorSubSystem::onAttach() {
        auto &app{ clove::Application::get() };

        //Pop the physics layer from the application
        app.popSubSystem<clove::PhysicsSubSystem>();

        //Add the editor camera outside of the current scene
        if(editorCamera == clove::NullEntity) {
            auto *const entityManager{ app.getEntityManager() };

            editorCamera                                                                  = entityManager->create();
            entityManager->addComponent<clove::TransformComponent>(editorCamera).position = clove::vec3f{ 0.0f, 0.0f, -10.0f };
            entityManager->addComponent<clove::CameraComponent>(editorCamera, clove::Camera{ clove::Camera::ProjectionMode::Perspective });
        }

        loadScene();
    }

    clove::InputResponse EditorSubSystem::onInputEvent(clove::InputEvent const &inputEvent) {
        if(auto const *const mouseEvent{ std::get_if<clove::Mouse::Event>(&inputEvent) }) {
            if(mouseEvent->getButton() == clove::MouseButton::Right) {
                if(mouseEvent->getType() == clove::Mouse::Event::Type::Pressed) {
                    //Make sure prev + pos gets updated when the event happens.
                    //This fixes situations where the mouse will be up, move and then go down
                    mousePos     = mouseEvent->getPos();
                    prevMousePos = mouseEvent->getPos();

                    moveMouse = true;
                    return clove::InputResponse::Consumed;
                } else {
                    moveMouse = false;
                    return clove::InputResponse::Consumed;
                }
            }

            if(mouseEvent->getType() == clove::Mouse::Event::Type::Move && moveMouse) {
                mousePos = mouseEvent->getPos();
                return clove::InputResponse::Consumed;
            }
        }
        return clove::InputResponse::Ignored;
    }

    void EditorSubSystem::onUpdate(clove::DeltaTime const deltaTime) {
        auto &keyBoard{ *clove::Application::get().getKeyboard() };
        auto &mouse{ *clove::Application::get().getMouse() };
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

    void EditorSubSystem::onDetach() {
        //saveScene();
        currentScene.destroyAllEntities();
    }

    void EditorSubSystem::saveScene() {
        currentScene.save(clove::Application::get().getFileSystem()->resolve("./scene.clvscene"));
    }

    void EditorSubSystem::loadScene() {
        currentScene.load(clove::Application::get().getFileSystem()->resolve("./scene.clvscene"));

        Engine_OnSceneLoaded ^ loadMessage { gcnew Engine_OnSceneLoaded };
        loadMessage->entities = gcnew System::Collections::Generic::List<Entity ^>{};
        for(auto entity : currentScene.getKnownEntities()) {
            Entity ^ editorEntity { gcnew Entity };
            editorEntity->id         = entity;
            editorEntity->name       = gcnew System::String(currentScene.getComponent<NameComponent>(entity).name.c_str());
            editorEntity->components = gcnew System::Collections::Generic::List<Component ^>{};

            //Add all of the component types for an entity
            /*if(currentScene.hasComponent<clove::TransformComponent>(entity)) {
                auto const &transform{ currentScene.getComponent<clove::TransformComponent>(entity) };

                auto const &pos{ transform.position };
                auto const &rot{ clove::quaternionToEuler(transform.rotation) };
                auto const &scale{ transform.scale };

                TransformComponentInitData ^ initData { gcnew TransformComponentInitData{} };
                initData->position = Vector3(pos.x, pos.y, pos.z);
                initData->rotation = Vector3(rot.x, rot.y, rot.z);
                initData->scale    = Vector3(scale.x, scale.y, scale.z);

                Component ^ componentData { gcnew Component{} };
                componentData->type = ComponentType::Transform;
                componentData->initData = initData;

                editorEntity->components->Add(componentData);
            }
            if(currentScene.hasComponent<clove::StaticModelComponent>(entity)) {
                StaticModelComponentInitData ^ initData { gcnew StaticModelComponentInitData{} };
                initData->meshPath     = gcnew System::String(currentScene.getComponent<clove::StaticModelComponent>(entity).model.getPath().c_str());
                initData->diffusePath  = gcnew System::String(currentScene.getComponent<clove::StaticModelComponent>(entity).material->getDiffuseTexture().getPath().c_str());
                initData->specularPath = gcnew System::String(currentScene.getComponent<clove::StaticModelComponent>(entity).material->getSpecularTexture().getPath().c_str());

                Component ^ componentData = gcnew Component{};
                componentData->type       = ComponentType::StaticModel;
                componentData->initData   = initData;

                editorEntity->components->Add(componentData);
            }
            if(currentScene.hasComponent<clove::PointLightComponent>(entity)) {
                Component ^ componentData = gcnew Component{};
                componentData->type       = ComponentType::PointLight;

                editorEntity->components->Add(componentData);
            }
            if(currentScene.hasComponent<clove::RigidBodyComponent>(entity)) {
                RigidBodyComponentInitData ^ initData = gcnew RigidBodyComponentInitData{};
                initData->mass                        = currentScene.getComponent<clove::RigidBodyComponent>(entity).mass;

                Component ^ componentData = gcnew Component{};
                componentData->type       = ComponentType::RigidBody;
                componentData->initData   = initData;

                editorEntity->components->Add(componentData);
            }
            if(currentScene.hasComponent<clove::CollisionShapeComponent>(entity)) {
                auto &component{ currentScene.getComponent<clove::CollisionShapeComponent>(entity) };

                CollisionShapeComponentInitData ^ initData { gcnew CollisionShapeComponentInitData{} };
                if(auto *sphere{ std::get_if<clove::CollisionShapeComponent::Sphere>(&component.shape) }) {
                    initData->shapeType = ShapeType::Sphere;
                    initData->radius    = sphere->radius;
                } else if(auto *cube{ std::get_if<clove::CollisionShapeComponent::Cube>(&component.shape) }) {
                    initData->shapeType   = ShapeType::Cube;
                    initData->halfExtents = Vector3(cube->halfExtents.x, cube->halfExtents.y, cube->halfExtents.z);
                }

                Component ^ componentData = gcnew Component{};
                componentData->type       = ComponentType::CollisionShape;
                componentData->initData   = initData;

                editorEntity->components->Add(componentData);
            }*/

            loadMessage->entities->Add(editorEntity);
        }

        MessageHandler::sendMessage(loadMessage);
    }

    clove::Entity EditorSubSystem::createEntity(std::string_view name) {
        clove::Entity entity{ currentScene.createEntity() };
        currentScene.addComponent<NameComponent>(entity, std::string{ std::move(name) });

        Engine_OnEntityCreated ^ message { gcnew Engine_OnEntityCreated };
        message->entity = entity;
        message->name   = gcnew System::String(std::string{ std::begin(name), std::end(name) }.c_str());
        MessageHandler::sendMessage(message);

        return entity;
    }

    void EditorSubSystem::deleteEntity(clove::Entity entity) {
        currentScene.deleteEntity(entity);

        Engine_OnEntityDeleted ^ message { gcnew Engine_OnEntityDeleted };
        message->entity = entity;
        MessageHandler::sendMessage(message);
    }

    void EditorSubSystem::addComponent(clove::Entity entity, std::string_view typeName) {
        clove::reflection::TypeInfo const *typeInfo{ clove::reflection::getTypeInfo(typeName) };
        CLOVE_ASSERT(typeInfo != nullptr);

        static size_t const transId{ typeid(clove::TransformComponent).hash_code() };
        static size_t const modelId{ typeid(clove::StaticModelComponent).hash_code() };
        static size_t const lightId{ typeid(clove::PointLightComponent).hash_code() };

        //TODO: Use switch statements

        uint8_t *componentMemory{ nullptr };
        bool added{ false };
        if(typeInfo->id == transId) {
            if(!currentScene.hasComponent<clove::TransformComponent>(entity)) {
                added           = true;
                componentMemory = reinterpret_cast<uint8_t *>(&currentScene.addComponent<clove::TransformComponent>(entity));
            }
        } else if(typeInfo->id == modelId) {
            if(!currentScene.hasComponent<clove::StaticModelComponent>(entity)) {
                added = true;
                auto &comp{ currentScene.addComponent<clove::StaticModelComponent>(entity) };

                //TEMP: loading in model manually for now
               
            }
        } else if(typeInfo->id == lightId) {
            if(!currentScene.hasComponent<clove::PointLightComponent>(entity)) {
                added           = true;
                componentMemory = reinterpret_cast<uint8_t *>(&currentScene.addComponent<clove::PointLightComponent>(entity));
            }
        }

        //TEMP: Asserting for now but if Clove/Membrane is not aware of the type then we need to pass off to user code
        //This can be a dynamically loaded function pointer
        CLOVE_ASSERT(added);

        auto message{ gcnew Engine_OnComponentAdded };
        message->entity        = entity;
        message->componentName = gcnew System::String{ typeName.data() };
        if(componentMemory != nullptr) {
            size_t constexpr bytes{ sizeof(clove::PointLightComponent) };
            array<System::Byte> ^ componentData = gcnew array<System::Byte>(bytes);
            for(size_t i{ 0 }; i < bytes; ++i) {
                componentData[i] = componentMemory[i];
            }
            message->data = componentData;
        }

        MessageHandler::sendMessage(message);
    }

    void EditorSubSystem::removeComponent(clove::Entity entity, std::string_view typeName) {
        clove::reflection::TypeInfo const *typeInfo{ clove::reflection::getTypeInfo(typeName) };
        CLOVE_ASSERT(typeInfo != nullptr);

        static size_t const transId{ typeid(clove::TransformComponent).hash_code() };
        static size_t const modelId{ typeid(clove::StaticModelComponent).hash_code() };
        static size_t const lightId{ typeid(clove::PointLightComponent).hash_code() };

        //TODO: Use switch statements

        bool removed{ false };
        if(typeInfo->id == transId) {
            currentScene.removeComponent<clove::TransformComponent>(entity);
            removed = true;
        } else if(typeInfo->id == modelId) {
            currentScene.removeComponent<clove::StaticModelComponent>(entity);
            removed = true;
        } else if(typeInfo->id == lightId) {
            currentScene.removeComponent<clove::PointLightComponent>(entity);
            removed = true;
        }

        CLOVE_ASSERT(removed);

        auto message{ gcnew Engine_OnComponentRemoved };
        message->entity        = entity;
        message->componentName = gcnew System::String{ typeName.data() };
        MessageHandler::sendMessage(message);
    }

    void EditorSubSystem::updateName(clove::Entity entity, std::string name) {
        if(currentScene.hasComponent<NameComponent>(entity)) {
            currentScene.getComponent<NameComponent>(entity).name = std::move(name);
        }
    }
}