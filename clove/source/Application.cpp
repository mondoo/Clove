#include "Clove/Application.hpp"

#include "Clove/InputEvent.hpp"
#include "Clove/Layer.hpp"
#include "Clove/Rendering/ForwardRenderer3D.hpp"
#include "Clove/Rendering/GraphicsImageRenderTarget.hpp"
#include "Clove/Rendering/SwapchainRenderTarget.hpp"
#include "Clove/Layers/AudioLayer.hpp"
#include "Clove/Layers/PhysicsLayer.hpp"
#include "Clove/Layers/RenderLayer.hpp"

#include <Clove/Audio/AudioDevice.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/ECS/EntityManager.hpp>
#include <Clove/Graphics/Graphics.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Log/Log.hpp>
#include <Clove/Platform/Platform.hpp>
#include <Clove/Platform/Window.hpp>

namespace garlic::clove {
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
        app->entityManager = std::make_unique<EntityManager>();

        //Layers
        app->pushLayer(std::make_shared<PhysicsLayer>(), LayerGroup::Initialisation);
        app->pushLayer(std::make_shared<AudioLayer>(), LayerGroup::Render);
        app->pushLayer(std::make_shared<RenderLayer>(), LayerGroup::Render);

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
        app->entityManager = std::make_unique<EntityManager>();

        //Layers
        app->pushLayer(std::make_shared<PhysicsLayer>(), LayerGroup::Initialisation);
        app->pushLayer(std::make_shared<AudioLayer>(), LayerGroup::Render);
        app->pushLayer(std::make_shared<RenderLayer>(), LayerGroup::Render);

        return { std::move(app), renderTargetPtr };
    }

    Application &Application::get() {
        return *instance;
    }

    void Application::pushLayer(std::shared_ptr<Layer> layer, LayerGroup group) {
        CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Attached layer: {0}", layer->getName());
        layer->onAttach();
        layers[group].push_back(std::move(layer));
    }

    void Application::popLayer(std::shared_ptr<Layer> const &layer) {
        for(auto &&[key, group] : layers) {
            if(auto it = std::find(group.begin(), group.end(), layer); it != group.end()) {
                CLOVE_LOG_DEBUG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Popped layer: {0}", (*it)->getName());
                (*it)->onDetach();
                group.erase(it);
                break;
            }
        }
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
                for(auto &&[key, group] : layers) {
                    for(auto &layer : group) {
                        if(layer->onInputEvent(event) == InputResponse::Consumed) {
                            break;
                        }
                    }
                }
            }
            while(auto mouseEvent = window->getMouse().getEvent()) {
                InputEvent const event{ *mouseEvent, InputEvent::Type::Mouse };
                for(auto &&[key, group] : layers) {
                    for(auto &layer : group) {
                        if(layer->onInputEvent(event) == InputResponse::Consumed) {
                            break;
                        }
                    }
                }
            }
        }

        for(auto &&[key, group] : layers) {
            for(auto &layer : group) {
                layer->onUpdate(deltaSeonds.count());
            }
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