#include "Membrane/RuntimeLayer.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/ModelLoader.hpp>

namespace garlic::membrane {
    RuntimeLayer::RuntimeLayer()
        : clove::Layer{ "Runtime Layer" }
        , ecsWorld{ clove::Application::get().getECSWorld() } {}

    void RuntimeLayer::onAttach() {
        auto lightEnt{ ecsWorld->create() };
        ecsWorld->addComponent<clove::TransformComponent>(lightEnt)->setPosition({ 5.0f, 0.0f, 0.0f });
        ecsWorld->addComponent<clove::PointLightComponent>(lightEnt);

        runtimeEntities.push_back(lightEnt);
    }

    void RuntimeLayer::onDetach() {
        for(auto &entity : runtimeEntities) {
            ecsWorld->destroy(entity);
        }
    }

    clove::Entity RuntimeLayer::addEntity() {
        auto cubeEnt{ ecsWorld->create() };
        runtimeEntities.push_back(cubeEnt);
        
        //TEMP: Add some components for debugging
        ecsWorld->addComponent<clove::TransformComponent>(cubeEnt);
        ecsWorld->addComponent<clove::StaticModelComponent>(cubeEnt, clove::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj"));

        return cubeEnt;
    }

    void RuntimeLayer::removeEntity(clove::Entity entity) {
        auto entityIter{ std::find(std::begin(runtimeEntities), std::end(runtimeEntities), entity) };
        if(entityIter != std::end(runtimeEntities)){
            runtimeEntities.erase(entityIter);
        }
    }
}