#include "Membrane/EditorLayer.hpp"

#include "Membrane/MessageHandler.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/CameraComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/ModelLoader.hpp>

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
    }

    void EditorLayer::onDetach() {
        clove::Application::get().getEntityManager()->destroy(editorCamera);
    }
}