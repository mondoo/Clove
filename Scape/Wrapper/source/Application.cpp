#include "Scape/Wrapper/Application.hpp"

#include <Clove/ECS/Entity.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/TransferCommandBuffer.hpp>
#include <Clove/Graphics/TransferQueue.hpp>
#include <Clove/Log/Log.hpp>
#include <Stem/Application.hpp>
#include <Stem/Components/CameraComponent.hpp>
#include <Stem/Components/PointLightComponent.hpp>
#include <Stem/Components/StaticModelComponent.hpp>
#include <Stem/Components/TransformComponent.hpp>
#include <Stem/Layer.hpp>
#include <Stem/ModelLoader.hpp>
#include <Stem/Rendering/Camera.hpp>
#include <Stem/Rendering/GraphicsImageRenderTarget.hpp>
#include <Stem/Systems/RenderSystem.hpp>

class TestLayer : public garlic::Layer {
private:
    blb::ecs::Entity cubeEnt;
    blb::ecs::Entity camEnt;

    garlic::Viewport viewport;

public:
    TestLayer(vec2ui size)
        : viewport{ 0, 0, static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) } {
    }

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

        camEnt = world->createEntity();
        camEnt.addComponent<garlic::TransformComponent>()->setPosition({ 0.0f, 0.0f, -10.0f });
        camEnt.addComponent<garlic::CameraComponent>(garlic::Camera{ viewport, garlic::Camera::ProjectionMode::Perspective });
    }

    void onUpdate(DeltaTime const deltaTime) override {
        static float time{ 0.0f };
        time += deltaTime;

        cubeEnt.getComponent<garlic::TransformComponent>()->setPosition({ 0.0f, sin(time), 0.0f });
    }

    void onDetach() override {
        garlic::Application::get().getECSWorld()->destroyAllEntites();
    }

    void resize(vec2ui size) {
        viewport.width  = size.x;
        viewport.height = size.y;
        camEnt.getComponent<garlic::CameraComponent>()->setViewport(viewport);
    }
};

struct AppWrapper {
    AppWrapper(garlic::clove::GraphicsApi graphicsApi, clv::AudioAPI audioApi, garlic::clove::GraphicsImage::Descriptor renderTargetImageDesc) {
        auto [app, rt] = garlic::Application::createHeadless(graphicsApi, audioApi, std::move(renderTargetImageDesc));
        this->app      = std::move(app);
        this->rt       = std::move(rt);
    }

    std::unique_ptr<garlic::Application> app;
    garlic::GraphicsImageRenderTarget *rt;
};

struct LayerWrapper {
    std::shared_ptr<TestLayer> layer;
};

namespace wrapper {
    Application::Application(int const width, int const height)
        : width{ width }
        , height{ height } {
        using namespace garlic::clove;

        //Hard coding format to B8G8R8A8_SRGB
        GraphicsImage::Descriptor const renderTargetImageDescriptor{
            .type        = GraphicsImage::Type::_2D,
            .usageFlags  = GraphicsImage::UsageMode::ColourAttachment | GraphicsImage::UsageMode::TransferSource,
            .dimensions  = { width, height },
            .format      = GraphicsImage::Format::B8G8R8A8_SRGB,
            .sharingMode = SharingMode::Concurrent,
        };
        appWrapper = std::make_unique<AppWrapper>(garlic::clove::GraphicsApi::Vulkan, clv::AudioAPI::OpenAl, std::move(renderTargetImageDescriptor));

        layerWrapper        = std::make_unique<LayerWrapper>();
        layerWrapper->layer = std::make_shared<TestLayer>();
        appWrapper->app->pushLayer(layerWrapper->layer);
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

    void Application::resize(int const width, int const height) {
        appWrapper->rt->resize({ width, height });
        layerWrapper->layer->resize({ width, height });

        this->width  = width;
        this->height = height;
    }

    void Application::copyRenderTargetToPointer(void *ptr) {
        auto const renderTargetBuffer = appWrapper->rt->getNextReadyBuffer();
        size_t constexpr bbp{ 4 };
        renderTargetBuffer->read(ptr, 0, width * height * bbp);
    }
}