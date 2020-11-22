#include "Scape/Wrapper/Application.hpp"

#include <Bulb/ECS/Entity.hpp>
#include <Bulb/ECS/World.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Graphics/TransferCommandBuffer.hpp>
#include <Clove/Graphics/TransferQueue.hpp>
#include <Root/Log/Log.hpp>
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
    TestLayer(clv::mth::vec2ui size)
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

    void onUpdate(clv::utl::DeltaTime const deltaTime) override {
        static float time{ 0.0f };
        time += deltaTime;

        cubeEnt.getComponent<garlic::TransformComponent>()->setPosition({ 0.0f, sin(time), 0.0f });
    }

    void onDetach() override {
        garlic::Application::get().getECSWorld()->destroyAllEntites();
    }

    void resize(clv::mth::vec2ui size) {
        viewport.width = size.x;
        viewport.height = size.y;
        camEnt.getComponent<garlic::CameraComponent>()->setViewport(viewport);
    }
};

struct AppWrapper {
    AppWrapper(clv::gfx::API graphicsApi, clv::AudioAPI audioApi, clv::gfx::GraphicsImage::Descriptor renderTargetImageDesc) {
        auto [app, rt] = garlic::createHeadlessApplication(graphicsApi, audioApi, std::move(renderTargetImageDesc));
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
        using namespace clv::gfx;

        auto const factory{ garlic::Application::get().getGraphicsDevice()->getGraphicsFactory() };

        auto const transferFinishedFence{ factory->createFence({}) };
        auto const transferQueue{ factory->createTransferQueue(CommandQueueDescriptor{ .flags = QueueFlags::Transient }) };
        std::shared_ptr<TransferCommandBuffer> const transferBuffer{ transferQueue->allocateCommandBuffer() };

        size_t constexpr bytesPerPixel{ 4 };
        size_t const bufferSize{ width * height * bytesPerPixel };
        auto const renderTargetBuffer = factory->createBuffer(GraphicsBuffer::Descriptor{
            .size        = bufferSize,
            .usageFlags  = GraphicsBuffer::UsageMode::TransferDestination,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::SystemMemory,
        });

        auto const renderTargetImage = appWrapper->rt->getNextReadyImage();

        ImageMemoryBarrierInfo constexpr layoutTransferInfo{
            .sourceAccess      = AccessFlags::None,
            .destinationAccess = AccessFlags::TransferWrite,
            .oldImageLayout    = GraphicsImage::Layout::Undefined,
            .newImageLayout    = GraphicsImage::Layout::TransferSourceOptimal,
            .sourceQueue       = QueueType::None,
            .destinationQueue  = QueueType::None,
        };

        transferBuffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        transferBuffer->imageMemoryBarrier(*renderTargetImage, std::move(layoutTransferInfo), PipelineObject::Stage::Top, PipelineObject::Stage::Transfer);
        transferBuffer->copyImageToBuffer(*renderTargetImage, { 0, 0, 0 }, { width, height, 1 }, *renderTargetBuffer, 0);
        transferBuffer->endRecording();

        transferQueue->submit({ TransferSubmitInfo{ .commandBuffers = { transferBuffer } } }, transferFinishedFence.get());
        transferFinishedFence->wait();
        transferQueue->freeCommandBuffer(*transferBuffer);

        renderTargetBuffer->read(ptr, 0, bufferSize);
    }
}