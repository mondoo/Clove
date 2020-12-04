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

        auto lightEnt{ world->create() };
        world->addComponent<clove::TransformComponent>(lightEnt)->setPosition({ 5.0f, 0.0f, 0.0f });
        world->addComponent<clove::PointLightComponent>(lightEnt);
    }

    void RuntimeLayer::onDetach() {
        auto *const world{ clove::Application::get().getECSWorld() };

        for(auto &entity : runtimeEntities) {
            world->destroy(entity);
        }
    }

    void RuntimeLayer::addEntity() {
        auto *const world{ clove::Application::get().getECSWorld() };

        auto cubeEnt{ world->create() };
        world->addComponent<clove::TransformComponent>(cubeEnt);
        world->addComponent<clove::StaticModelComponent>(cubeEnt, clove::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj"));

        runtimeEntities.push_back(cubeEnt);
    }
}