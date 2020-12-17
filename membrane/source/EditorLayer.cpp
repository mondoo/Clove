#include "Membrane/EditorLayer.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/CameraComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/ModelLoader.hpp>

namespace garlic::membrane {
    EditorLayer::EditorLayer(clove::vec2ui size)
        : clove::Layer("Editor Layer")
        , viewport{ 0, 0, static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) } {
    }

    void EditorLayer::onAttach() {
        auto *const world{ clove::Application::get().getECSWorld() };

        editorCamera = world->create();
        world->addComponent<clove::TransformComponent>(editorCamera)->setPosition({ 0.0f, 0.0f, -10.0f });
        world->addComponent<clove::CameraComponent>(editorCamera, clove::Camera{ viewport, clove::Camera::ProjectionMode::Perspective });
    }

    void EditorLayer::onUpdate(clove::DeltaTime const deltaTime) {
        /*
        update view logic in here?

        Could have a system that just goes through all transform components and updates them
            - Not the most scalable solution by any means
        */
    }

    void EditorLayer::onDetach() {
        clove::Application::get().getECSWorld()->destroy(editorCamera);
    }

    void EditorLayer::resizeViewport(clove::vec2ui size) {
        viewport.width  = size.x;
        viewport.height = size.y;
        clove::Application::get().getECSWorld()->getComponent<clove::CameraComponent>(editorCamera)->setViewport(viewport);
    }
}