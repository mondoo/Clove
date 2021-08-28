#include "Membrane/RuntimeSubSystem.hpp"

#include "Membrane/MessageHandler.hpp"
#include "Membrane/Messages.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Maths/MathsHelpers.hpp>
#include <Clove/ModelLoader.hpp>
#include <Clove/SubSystems/PhysicsSubSystem.hpp>

namespace membrane {
    RuntimeSubSystem::RuntimeSubSystem()
        : clove::SubSystem{ "Runtime SubSystem" }
        , currentScene{ clove::Application::get().getEntityManager() } {
    }

    void RuntimeSubSystem::onAttach() {
        auto &app{ clove::Application::get() };

        //push the physics sub system from the application
        app.pushSubSystem<clove::PhysicsSubSystem>(clove::Application::SubSystemGroup::Initialisation, app.getEntityManager());

        currentScene.load(app.getFileSystem()->resolve("./scene.clvscene"));
    }

    void RuntimeSubSystem::onUpdate(clove::DeltaTime const deltaTime) {
        //Make sure any movement is reflected in editor
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
    }

    void RuntimeSubSystem::onDetach() {
        currentScene.destroyAllEntities();
    }
}