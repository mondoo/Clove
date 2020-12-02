#include "Membrane/Application.hpp"

#include <Clove/Application.hpp>
#include <Clove/Audio/Audio.hpp>
#include <Clove/Components/CameraComponent.hpp>
#include <Clove/Components/PointLightComponent.hpp>
#include <Clove/Components/StaticModelComponent.hpp>
#include <Clove/Components/TransformComponent.hpp>
#include <Clove/ECS/Entity.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <Clove/Graphics/GraphicsBuffer.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
#include <Clove/Layer.hpp>
#include <Clove/Log/Log.hpp>
#include <Clove/ModelLoader.hpp>
#include <Clove/Rendering/Camera.hpp>
#include <Clove/Rendering/GraphicsImageRenderTarget.hpp>
#include <Clove/Systems/RenderSystem.hpp>

namespace garlic::membrane {
    class TestLayer : public clove::Layer {
    private:
        clove::Entity cubeEnt;
        clove::Entity camEnt;

        clove::Viewport viewport;

    public:
        TestLayer(clove::vec2ui size)
            : viewport{ 0, 0, static_cast<int32_t>(size.x), static_cast<int32_t>(size.y) } {
            clove::Application::get().getECSWorld()->addSystem<clove::RenderSystem>();
        }

        void onAttach() override {
            auto *const world = clove::Application::get().getECSWorld();

            cubeEnt = world->create();
            world->addComponent<clove::TransformComponent>(cubeEnt);
            world->addComponent<clove::StaticModelComponent>(cubeEnt, clove::ModelLoader::loadStaticModel(ASSET_DIR "/cube.obj"));

            auto lightEnt = world->create();
            world->addComponent<clove::TransformComponent>(lightEnt)->setPosition({ 5.0f, 0.0f, 0.0f });
            world->addComponent<clove::PointLightComponent>(lightEnt);

            camEnt = world->create();
            world->addComponent<clove::TransformComponent>(camEnt)->setPosition({ 0.0f, 0.0f, -10.0f });
            world->addComponent<clove::CameraComponent>(camEnt, clove::Camera{ viewport, clove::Camera::ProjectionMode::Perspective });
        }

        void onUpdate(clove::DeltaTime const deltaTime) override {
            static float time{ 0.0f };
            time += deltaTime;

            clove::Application::get().getECSWorld()->getComponent<clove::TransformComponent>(cubeEnt)->setPosition({ 0.0f, sin(time), 0.0f });
        }

        void onDetach() override {
            clove::Application::get().getECSWorld()->destroyAll();
        }

        void resize(clove::vec2ui size) {
            viewport.width  = size.x;
            viewport.height = size.y;
            clove::Application::get().getECSWorld()->getComponent<clove::CameraComponent>(camEnt)->setViewport(viewport);
        }
    };

    class ConsoleLogger : public clove::Logger::Output {
    public:
        void doLog(clove::LogLevel level, std::string_view msg) override {
            //Conver to std::string as there seems to be issues when using std::data(msg)
            System::Console::WriteLine(gcnew System::String(std::string{ std::begin(msg), std::end(msg) }.c_str()));
        }
    };
}

namespace garlic::membrane {
    Application::Application(int const width, int const height)
        : width{ width }
        , height{ height } {
        using namespace garlic::clove;

        //Set the logger for Clove to redirect to System.Console
        Logger::setOutput(std::make_unique<ConsoleLogger>());

        //Hard coding format to B8G8R8A8_SRGB
        GraphicsImage::Descriptor renderTargetImageDescriptor{};
        renderTargetImageDescriptor.type        = GraphicsImage::Type::_2D;
        renderTargetImageDescriptor.usageFlags  = GraphicsImage::UsageMode::ColourAttachment | GraphicsImage::UsageMode::TransferSource;
        renderTargetImageDescriptor.dimensions  = { width, height };
        renderTargetImageDescriptor.format      = GraphicsImage::Format::B8G8R8A8_SRGB;
        renderTargetImageDescriptor.sharingMode = SharingMode::Concurrent;

        //Use pair as there seems to be an issue when using structured bindings
        auto pair{ clove::Application::createHeadless(GraphicsApi::Vulkan, AudioApi::OpenAl, std::move(renderTargetImageDescriptor)) };
        app          = pair.first.release();
        renderTarget = pair.second;

        layer  = new std::shared_ptr<TestLayer>();
        *layer = std::make_shared<TestLayer>(clove::vec2ui{ width, height });
        app->pushLayer(*layer);
    }

    Application::~Application() {
        this->!Application();
    }

    Application::!Application() {
        delete app;
        delete layer;
    }

    bool Application::isRunning() {
        return app->getState() != clove::Application::State::Stopped;
    }

    void Application::tick() {
        app->tick();
    }

    void Application::shutdown() {
        app->shutdown();
    }

    void Application::resize(int width, int height) {
        renderTarget->resize({ width, height });
        (*layer)->resize({ width, height });

        this->width  = width;
        this->height = height;
    }

    void Application::render(System::IntPtr backBuffer) {
        auto const renderTargetBuffer = renderTarget->getNextReadyBuffer();
        size_t constexpr bbp{ 4 };
        renderTargetBuffer->read(backBuffer.ToPointer(), 0, width * height * bbp);
    }
}