#include "Membrane/EditorLayer.hpp"

#include "Membrane/MessageHandler.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/CameraComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/ModelLoader.hpp>
#include <Clove/Surface.hpp>

namespace garlic::membrane {
    EditorLayer::EditorLayer()
        : clove::Layer{ "Editor Layer" } {
    }

    void EditorLayer::onAttach() {
        auto *const entityManager{ clove::Application::get().getEntityManager() };

        editorCamera                                                                  = entityManager->create();
        entityManager->addComponent<clove::TransformComponent>(editorCamera).position = clove::vec3f{ 0.0f, 0.0f, -10.0f };
        entityManager->addComponent<clove::CameraComponent>(editorCamera, clove::Camera{ clove::Camera::ProjectionMode::Perspective });
    }

    clove::InputResponse EditorLayer::onInputEvent(clove::InputEvent const &inputEvent) {
        return clove::InputResponse::Ignored;
    }

    void EditorLayer::onUpdate(clove::DeltaTime const deltaTime) {
        clove::vec3f pos{};
        auto &keyBoard{ clove::Application::get().getSurface()->getKeyboard() };
        auto *const entityManager{ clove::Application::get().getEntityManager() };

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
        if(keyBoard.isKeyPressed(clove::Key::Space)) {
            pos.y += 1.0f;
        }
        if(keyBoard.isKeyPressed(clove::Key::Shift_Left)) {
            pos.y -= 1.0f;
        }

        auto &camTrans{ entityManager->getComponent<clove::TransformComponent>(editorCamera) };

        camTrans.position += camTrans.rotation * pos * 10.0f * deltaTime.getDeltaSeconds();
    }

    void EditorLayer::onDetach() {
        clove::Application::get().getEntityManager()->destroy(editorCamera);
    }
}