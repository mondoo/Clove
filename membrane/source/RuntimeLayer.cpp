#include "Membrane/RuntimeLayer.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/ModelLoader.hpp>

namespace garlic::membrane {
    void RuntimeLayer::onAttach() {
        auto *const world{ clove::Application::get().getECSWorld() };

        auto lightEnt{ world->createEntity() };
        lightEnt.addComponent<clove::TransformComponent>()->setPosition({ 5.0f, 0.0f, 0.0f });
        lightEnt.addComponent<clove::PointLightComponent>();
    }

    void RuntimeLayer::onDetach() {
        auto *const world{ clove::Application::get().getECSWorld() };

        for(auto &entity : runtimeEntities) {
            world->destroyEntity(entity);
        }
    }

    void RuntimeLayer::addEntity() {
        auto cubeEnt = clove::Application::get().getECSWorld()->createEntity();
        cubeEnt.addComponent<clove::TransformComponent>();
        cubeEnt.addComponent<clove::StaticModelComponent>(clove::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj"));

        runtimeEntities.push_back(cubeEnt);
    }
}