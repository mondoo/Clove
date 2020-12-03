#include "Membrane/EditorLayer.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/CameraComponent.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/ModelLoader.hpp>
#include <Clove/Systems/RenderSystem.hpp>

namespace garlic::membrane {
    EditorLayer::EditorLayer(clove::vec2ui size)
        : viewport{ 0, 0, static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) } {
    }

    void EditorLayer::onAttach() {
        auto *const world = clove::Application::get().getECSWorld();

        auto lightEnt = world->createEntity();
        lightEnt.addComponent<clove::TransformComponent>()->setPosition({ 5.0f, 0.0f, 0.0f });
        lightEnt.addComponent<clove::PointLightComponent>();

        camEnt = world->createEntity();
        camEnt.addComponent<clove::TransformComponent>()->setPosition({ 0.0f, 0.0f, -10.0f });
        camEnt.addComponent<clove::CameraComponent>(clove::Camera{ viewport, clove::Camera::ProjectionMode::Perspective });
    }

    void EditorLayer::onUpdate(clove::DeltaTime const deltaTime) {
        
    }

    void EditorLayer::onDetach() {
        clove::Application::get().getECSWorld()->destroyAllEntites();
    }

    void EditorLayer::resizeViewport(clove::vec2ui size) {
        viewport.width  = size.x;
        viewport.height = size.y;
        camEnt.getComponent<clove::CameraComponent>()->setViewport(viewport);
    }

    void EditorLayer::addEntity(){
        auto cubeEnt = clove::Application::get().getECSWorld()->createEntity();
        cubeEnt.addComponent<clove::TransformComponent>();
        cubeEnt.addComponent<clove::StaticModelComponent>(clove::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj"));
    }
}