#include "Membrane/RuntimeLayer.hpp"

#include <Clove/Application.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/EntityManager.hpp>

namespace garlic::membrane {
    RuntimeLayer::RuntimeLayer()
        : clove::Layer{ "Runtime Layer" }
        , entityManager{ clove::Application::get().getEntityManager() } {}

    void RuntimeLayer::onAttach() {
        auto lightEnt{ entityManager->create() };
        entityManager->addComponent<clove::TransformComponent>(lightEnt)->setPosition({ 5.0f, 0.0f, 0.0f });
        entityManager->addComponent<clove::PointLightComponent>(lightEnt);

        runtimeEntities.push_back(lightEnt);
    }

    void RuntimeLayer::onDetach() {
        for(auto &entity : runtimeEntities) {
            entityManager->destroy(entity);
        }
    }

    clove::Entity RuntimeLayer::addEntity() {
        auto entity{ entityManager->create() };
        runtimeEntities.push_back(entity);
        return entity;
    }

    void RuntimeLayer::removeEntity(clove::Entity entity) {
        auto entityIter{ std::find(std::begin(runtimeEntities), std::end(runtimeEntities), entity) };
        if(entityIter != std::end(runtimeEntities)){
            runtimeEntities.erase(entityIter);
        }
    }
}