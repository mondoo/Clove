#include "Scape/Wrapper/Application.hpp"

#include <Bulb/ECS/Entity.hpp>
#include <Bulb/ECS/World.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Stem/Application.hpp>
#include <Stem/Components/CameraComponent.hpp>
#include <Stem/Components/PointLightComponent.hpp>
#include <Stem/Components/StaticModelComponent.hpp>
#include <Stem/Components/TransformComponent.hpp>
#include <Stem/Layer.hpp>
#include <Stem/ModelLoader.hpp>
#include <Stem/Systems/RenderSystem.hpp>
#include <Stem/Rendering/Camera.hpp>
#include <Stem/Rendering/GraphicsImageRenderTarget.hpp>

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

struct AppWrapper {
    AppWrapper(clv::gfx::API graphicsApi, clv::AudioAPI audioApi, clv::gfx::GraphicsImage::Descriptor renderTargetImageDesc){
        auto [app, rt] = garlic::createHeadlessApplication(graphicsApi, audioApi, std::move(renderTargetImageDesc));
        this->app      = std::move(app);
        this->rt       = std::move(rt);
    }

    std::unique_ptr<garlic::Application> app;
    garlic::GraphicsImageRenderTarget *rt;
};

namespace wrapper {
    Application::Application(int const width, int const height)
        : width{ width }
        , height{ height } {
        using namespace clv::gfx;
        
        //Hard coding format to B8G8R8A8_SRGB
        GraphicsImage::Descriptor const renderTargetImageDescriptor{
            .type        = GraphicsImage::Type::_2D,
            .usageFlags  = GraphicsImage::UsageMode::ColourAttachment | GraphicsImage::UsageMode::TransferSource,
            .dimensions  = { width, height },
            .format      = GraphicsImage::Format::B8G8R8A8_SRGB,
            .sharingMode = SharingMode::Concurrent,
        };
        appWrapper = std::make_unique<AppWrapper>(clv::gfx::API::Vulkan, clv::AudioAPI::OpenAl, std::move(renderTargetImageDescriptor));
    }

    Application::~Application() = default;

    bool Application::isRunning() {
        return appWrapper->app->getState() == garlic::Application::State::Running;
    }

    void Application::tick() {
        appWrapper->app->tick();
    }

    void Application::shutdown() {
        appWrapper->app->shutdown();
    }
    }
}