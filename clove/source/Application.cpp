#include "Clove/Application.hpp"

#include "Clove/InputEvent.hpp"
#include "Clove/Layer.hpp"
#include "Clove/Rendering/ForwardRenderer3D.hpp"
#include "Clove/Rendering/GraphicsImageRenderTarget.hpp"
#include "Clove/Rendering/SwapchainRenderTarget.hpp"
#include "Clove/Systems/AudioSystem.hpp"
#include "Clove/Systems/PhysicsSystem.hpp"
#include "Clove/Systems/RenderSystem.hpp"

#include <Clove/Audio/AudioDevice.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/ECS/World.hpp>
#include <Clove/Graphics/Graphics.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Log/Log.hpp>
#include <Clove/Platform/Platform.hpp>
#include <Clove/Platform/Window.hpp>

namespace garlic::clove {
    namespace {
        class ApplicationLayer : public Layer {
            AudioSystem audioSystem{};
            PhysicsSystem physicsSystem{};
            RenderSystem renderSystem{};

            World *ecsWorld{ nullptr };

        public:
            ApplicationLayer(World *ecsWorld)
                : Layer("Application")
                , ecsWorld(ecsWorld) {
            }

            void onUpdate(DeltaTime const deltaTime) override {
                audioSystem.update(*ecsWorld);
                physicsSystem.update(*ecsWorld, deltaTime);
                renderSystem.update(*ecsWorld, deltaTime);
            }
        };
    }

    Application *Application::instance{ nullptr };

    Application::~Application() = default;

    std::unique_ptr<Application> Application::create(GraphicsApi graphicsApi, AudioApi audioApi, WindowDescriptor windowDescriptor) {
        std::unique_ptr<Application> app{ new Application };//Initialise without make_unique because we can only access the ctor here

        //Platform
        app->window = Platform::createWindow(std::move(windowDescriptor));

        //Graphics
        app->graphicsDevice = createGraphicsDevice(graphicsApi, app->window->getNativeWindow());
        app->renderer       = std::make_unique<ForwardRenderer3D>(std::make_unique<SwapchainRenderTarget>());

        //Audio
        app->audioDevice = createAudioDevice(audioApi);

        //ECS
        app->world = std::make_unique<World>();

        //Layer
        app->layerStack.pushLayer(std::make_shared<ApplicationLayer>(app->world.get()));

        return app;
    }

    std::pair<std::unique_ptr<Application>, GraphicsImageRenderTarget *> Application::createHeadless(GraphicsApi graphicsApi, AudioApi audioApi, GraphicsImage::Descriptor renderTargetDescriptor) {
        std::unique_ptr<Application> app{ new Application };//Initialise without make_unique because we can only access the ctor here

        //Graphics
        app->graphicsDevice = createGraphicsDevice(graphicsApi, std::any{});

        auto renderTarget{ std::make_unique<GraphicsImageRenderTarget>(std::move(renderTargetDescriptor)) };
        auto *renderTargetPtr{ renderTarget.get() };
        app->renderer = std::make_unique<ForwardRenderer3D>(std::move(renderTarget));

        //Audio
        app->audioDevice = createAudioDevice(audioApi);

        //ECS
        app->world = std::make_unique<World>();

        //Layer
        app->layerStack.pushLayer(std::make_shared<ApplicationLayer>(app->world.get()));

        return { std::move(app), renderTargetPtr };
    }

    Application &Application::get() {
        return *instance;
    }

    void Application::pushLayer(std::shared_ptr<Layer> layer) {
        layerStack.pushLayer(std::move(layer));
    }

    void Application::pushOverlay(std::shared_ptr<Layer> overlay) {
        layerStack.pushOverlay(std::move(overlay));
    }

    Application::State Application::getState() const {
        if(window != nullptr) {
            return window->isOpen() ? State::Running : State::Stopped;
        } else {
            return State::Running;
        }
    }

    void Application::tick() {
        if(getState() != State::Running) {
            return;
        }

        auto const currFrameTime{ std::chrono::system_clock::now() };
        std::chrono::duration<float> const deltaSeonds{ currFrameTime - prevFrameTime };
        prevFrameTime = currFrameTime;

        renderer->begin();

        if(window != nullptr) {
            window->processInput();

            while(auto keyEvent = window->getKeyboard().getKeyEvent()) {
                InputEvent const event{ *keyEvent, InputEvent::Type::Keyboard };
                for(auto const &layer : layerStack) {
                    if(layer->onInputEvent(event) == InputResponse::Consumed) {
                        break;
                    }
                }
            }
            while(auto mouseEvent = window->getMouse().getEvent()) {
                InputEvent const event{ *mouseEvent, InputEvent::Type::Mouse };
                for(auto const &layer : layerStack) {
                    if(layer->onInputEvent(event) == InputResponse::Consumed) {
                        break;
                    }
                }
            }
        }

        for(auto const &layer : layerStack) {
            layer->onUpdate(deltaSeonds.count());
        }

        renderer->end();
    }

    void Application::shutdown() {
        graphicsDevice->waitForIdleDevice();
        if(window != nullptr) {
            window->close();
        }
    }

    Application::Application() {
        CLOVE_ASSERT(instance == nullptr, "Only one Application can be active");
        instance = this;

        prevFrameTime = std::chrono::system_clock::now();
    }
}