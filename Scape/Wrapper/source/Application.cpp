#include "Scape/Wrapper/Application.hpp"

#include <Bulb/ECS/Entity.hpp>
#include <Bulb/ECS/World.hpp>
#include <Stem/Application.hpp>
#include <Stem/Components/CameraComponent.hpp>
#include <Stem/Components/PointLightComponent.hpp>
#include <Stem/Components/StaticModelComponent.hpp>
#include <Stem/Components/TransformComponent.hpp>
#include <Stem/Layer.hpp>
#include <Stem/ModelLoader.hpp>
#include <Stem/Systems/RenderSystem.hpp>
#include <Stem/Rendering/Camera.hpp>

class TestLayer : public garlic::Layer {
private:
    blb::ecs::Entity cubeEnt;

public:
    TestLayer() {
        garlic::Application::get().getECSWorld()->addSystem<garlic::RenderSystem>();
    }

    void onAttach() override {
        auto *const world = garlic::Application::get().getECSWorld();

        cubeEnt = world->createEntity();
        cubeEnt.addComponent<garlic::TransformComponent>();
        cubeEnt.addComponent<garlic::StaticModelComponent>(garlic::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj"));

        auto lightEnt = world->createEntity();
        lightEnt.addComponent<garlic::TransformComponent>()->setPosition({ 5.0f, 0.0f, 0.0f });
        lightEnt.addComponent<garlic::PointLightComponent>();

        auto camEnt = world->createEntity();
        camEnt.addComponent<garlic::TransformComponent>()->setPosition({ 0.0f, 0.0f, -10.0f });
        camEnt.addComponent<garlic::CameraComponent>(garlic::Camera{ garlic::Camera::ProjectionMode::Perspective });
    }

    void onUpdate(clv::utl::DeltaTime const deltaTime) override {
        static float time{ 0.0f };
        time += deltaTime;

        cubeEnt.getComponent<garlic::TransformComponent>()->setPosition({ 0.0f, sin(time), 0.0f });
    }

    void onDetach() override {
        garlic::Application::get().getECSWorld()->destroyAllEntites();
    }
};

garlic::Application::Descriptor getApplicationDescriptor() {
    return garlic::Application::Descriptor{
        .windowDescriptor = {
            .title  = "Temp",
            .width  = 800,
            .height = 600,
        },
    };
}

std::shared_ptr<garlic::Layer>
createApplicationLayer(garlic::Application const &app) {
    return std::make_shared<TestLayer>();
}

struct AppWrapper {
    garlic::Application app{};
};

namespace wrapper {
    Application::Application() {
        appWrapper = std::make_unique<AppWrapper>();
    }

    Application::~Application() = default;

    void Application::shutdown() {
        appWrapper->app.shutdown();
    }

    bool Application::tick() {
        if(appWrapper->app.getState() == garlic::Application::State::Running) {
            appWrapper->app.tick();
            return true;
        } else {
            return false;
        }
    }
}