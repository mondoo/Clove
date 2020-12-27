#include "Membrane/EditorLayer.hpp"

#include "Membrane/MessageHandler.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/CameraComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/ModelLoader.hpp>

namespace garlic::membrane {
    EditorLayer::EditorLayer(clove::vec2ui size)
        : clove::Layer("Editor Layer")
        , viewport{ 0, 0, static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) } {
    }

    void EditorLayer::onAttach() {
        auto *const entityManager{ clove::Application::get().getEntityManager() };

        editorCamera = entityManager->create();
        entityManager->addComponent<clove::TransformComponent>(editorCamera)->setPosition({ 0.0f, 0.0f, -10.0f });
        entityManager->addComponent<clove::CameraComponent>(editorCamera, clove::Camera{ viewport, clove::Camera::ProjectionMode::Perspective });
    }

    void EditorLayer::onUpdate(clove::DeltaTime const deltaTime) {
    }

    void EditorLayer::onDetach() {
        clove::Application::get().getEntityManager()->destroy(editorCamera);
    }

    void EditorLayer::resizeViewport(clove::vec2ui size) {
        viewport.width  = size.x;
        viewport.height = size.y;
        clove::Application::get().getEntityManager()->getComponent<clove::CameraComponent>(editorCamera)->setViewport(viewport);
    }
}