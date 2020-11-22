#include "Stem/Application.hpp"

#include "Stem/InputEvent.hpp"
#include "Stem/Layer.hpp"
#include "Stem/Rendering/ForwardRenderer3D.hpp"
#include "Stem/Rendering/GraphicsImageRenderTarget.hpp"
#include "Stem/Rendering/SwapchainRenderTarget.hpp"

#include <Bulb/ECS/World.hpp>
#include <Clove/Audio/AudioFactory.hpp>
#include <Clove/Graphics/Graphics.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Platform/Platform.hpp>
#include <Clove/Platform/Window.hpp>
#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

namespace garlic::inline stem {
    Application *Application::instance{ nullptr };

    Application::~Application() = default;

    std::unique_ptr<Application> createApplication(clv::gfx::API graphicsApi, clv::AudioAPI audioApi, clv::plt::WindowDescriptor windowDescriptor) {
        std::unique_ptr<Application> app{ new Application };//Initialise without make_unique because we can only access the ctor here

        app->window = app->platformInstance->createWindow(std::move(windowDescriptor));

        //Platform
        app->platformInstance = clv::plt::createPlatformInstance();

        //Graphics
        app->graphicsDevice = clv::gfx::createGraphicsDevice(graphicsApi, app->window->getNativeWindow());
        app->renderer       = std::make_unique<ForwardRenderer3D>(std::make_unique<SwapchainRenderTarget>());

        //Audio
        app->audioFactory = clv::createAudioFactory(audioApi);

        app->world = std::make_unique<blb::ecs::World>();

        return app;
    }

    std::pair<std::unique_ptr<Application>, GraphicsImageRenderTarget *> createHeadlessApplication(clv::gfx::API graphicsApi, clv::AudioAPI audioApi, clv::gfx::GraphicsImage::Descriptor renderTargetDescriptor) {
        std::unique_ptr<Application> app{ new Application };//Initialise without make_unique because we can only access the ctor here

        //Platform
        app->platformInstance = clv::plt::createPlatformInstance();

        //Graphics
        app->graphicsDevice = clv::gfx::createGraphicsDevice(graphicsApi, std::any{});

        auto renderTarget{ std::make_unique<GraphicsImageRenderTarget>(std::move(renderTargetDescriptor)) };
        auto *renderTargetPtr{ renderTarget.get() };
        app->renderer = std::make_unique<ForwardRenderer3D>(std::move(renderTarget));

        //Audio
        app->audioFactory = clv::createAudioFactory(audioApi);

        app->world = std::make_unique<blb::ecs::World>();

        return std::make_pair(std::move(app), renderTargetPtr);
    }

    Application &Application::get() {
        return *instance;
    }

    void Application::pushLayer(std::shared_ptr<Layer> layer) {
        layerStack.pushLayer(std::move(layer));
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

        //Respond to input
        if(window != nullptr) {
            window->processInput();

            while(auto keyEvent = window->getKeyboard().getKeyEvent()) {
                InputEvent const event{ *keyEvent, InputEventType::Keyboard };
                for(auto const &layer : layerStack) {
                    if(layer->onInputEvent(event) == InputResponse::Consumed) {
                        break;
                    }
                }
            }
            while(auto mouseEvent = window->getMouse().getEvent()) {
                InputEvent const event{ *mouseEvent, InputEventType::Mouse };
                for(auto const &layer : layerStack) {
                    if(layer->onInputEvent(event) == InputResponse::Consumed) {
                        break;
                    }
                }
            }
        }

        //Do client logic
        for(auto const &layer : layerStack) {
            layer->onUpdate(deltaSeonds.count());
        }

        renderer->begin();

        //Update ECS
        world->update(deltaSeonds.count());

        //Render
        renderer->end();
    }

    void Application::shutdown() {
        if(window != nullptr) {
            window->close();
        }
    }

    clv::gfx::GraphicsDevice *Application::getGraphicsDevice() const {
        return graphicsDevice.get();
    }

    clv::AudioFactory *Application::getAudioFactory() const {
        return audioFactory.get();
    }

    std::shared_ptr<clv::plt::Window> const &Application::getWindow() const {
        return window;
    }

    ForwardRenderer3D *Application::getRenderer() const {
        return renderer.get();
    }

    blb::ecs::World *Application::getECSWorld() const {
        return world.get();
    }

    Application::Application() {
        GARLIC_ASSERT(instance == nullptr, "Only one Application can be active");
        instance = this;

        prevFrameTime = std::chrono::system_clock::now();
    }
}