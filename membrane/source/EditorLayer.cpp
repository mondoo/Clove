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

        editorCamera = world->createEntity();
        editorCamera.addComponent<clove::TransformComponent>()->setPosition({ 0.0f, 0.0f, -10.0f });
        editorCamera.addComponent<clove::CameraComponent>(clove::Camera{ viewport, clove::Camera::ProjectionMode::Perspective });
    }

    void EditorLayer::onDetach() {
        clove::Application::get().getECSWorld()->destroyEntity(editorCamera);
    }

    void EditorLayer::resizeViewport(clove::vec2ui size) {
        viewport.width  = size.x;
        viewport.height = size.y;
        editorCamera.getComponent<clove::CameraComponent>()->setViewport(viewport);
    }
}