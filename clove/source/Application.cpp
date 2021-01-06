#include "Clove/Application.hpp"

#include "Clove/InputEvent.hpp"
#include "Clove/Layer.hpp"
#include "Clove/Layers/AudioLayer.hpp"
#include "Clove/Layers/PhysicsLayer.hpp"
#include "Clove/Layers/RenderLayer.hpp"
#include "Clove/Layers/TransformLayer.hpp"
#include "Clove/Rendering/ForwardRenderer3D.hpp"
#include "Clove/Rendering/GraphicsImageRenderTarget.hpp"
#include "Clove/Rendering/SwapchainRenderTarget.hpp"
#include "Clove/WindowSurface.hpp"

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

        auto window{ Platform::createWindow(std::move(windowDescriptor)) };
        window->onWindowCloseDelegate.bind(&Application::shutdown, app.get());

        //Devices
        app->graphicsDevice = createGraphicsDevice(graphicsApi, window->getNativeWindow());
        app->audioDevice    = createAudioDevice(audioApi);

        app->surface = std::make_unique<WindowSurface>(std::move(window));

        //Systems
        app->renderer      = std::make_unique<ForwardRenderer3D>(std::make_unique<SwapchainRenderTarget>(*app->surface, app->graphicsDevice.get()));
        app->entityManager = std::make_unique<EntityManager>();

        //Layers
        app->physicsLayer = std::make_shared<PhysicsLayer>();
        app->pushLayer(std::make_shared<TransformLayer>(), LayerGroup::Initialisation);
        app->pushLayer(app->physicsLayer, LayerGroup::Initialisation);
        app->pushLayer(std::make_shared<AudioLayer>(), LayerGroup::Render);
        app->pushLayer(std::make_shared<RenderLayer>(), LayerGroup::Render);

        return app;
    }

    std::pair<std::unique_ptr<Application>, GraphicsImageRenderTarget *> Application::createHeadless(GraphicsApi graphicsApi, AudioApi audioApi, GraphicsImage::Descriptor renderTargetDescriptor) {
        std::unique_ptr<Application> app{ new Application };//Initialise without make_unique because we can only access the ctor here

        //Devices
        app->graphicsDevice = createGraphicsDevice(graphicsApi, std::any{});
        app->audioDevice    = createAudioDevice(audioApi);

        //Systems
        app->renderer      = std::make_unique<ForwardRenderer3D>(std::make_unique<SwapchainRenderTarget>());
        app->entityManager = std::make_unique<EntityManager>();

        //Layers
        app->physicsLayer = std::make_shared<PhysicsLayer>();
        app->pushLayer(std::make_shared<TransformLayer>(), LayerGroup::Initialisation);
        app->pushLayer(app->physicsLayer, LayerGroup::Initialisation);
        app->pushLayer(std::make_shared<AudioLayer>(), LayerGroup::Render);
        app->pushLayer(std::make_shared<RenderLayer>(), LayerGroup::Render);

        auto renderTarget{ std::make_unique<GraphicsImageRenderTarget>(std::move(renderTargetDescriptor)) };
        auto *renderTargetPtr{ renderTarget.get() };
        app->renderer = std::make_unique<ForwardRenderer3D>(std::move(renderTarget));

        return { std::move(app), renderTargetPtr };
    }

    Application &Application::get() {
        return *instance;
    }

    void Application::pushLayer(std::shared_ptr<Layer> layer, LayerGroup group) {
        CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Attached layer: {0}", layer->getName());
        layer->onAttach();
        layers[group].push_back(std::move(layer));
    }

    void Application::popLayer(std::shared_ptr<Layer> const &layer) {
        for(auto &&[key, group] : layers) {
            if(auto it = std::find(group.begin(), group.end(), layer); it != group.end()) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Popped layer: {0}", (*it)->getName());
                (*it)->onDetach();
                group.erase(it);
                break;
            }
        }
    }

    void Application::tick() {
        //Process input first incase we get a close event.
        surface->processInput();

        if(currentState != State::Running) {
            return;
        }

        auto const currFrameTime{ std::chrono::system_clock::now() };
        std::chrono::duration<float> const deltaSeonds{ currFrameTime - prevFrameTime };
        prevFrameTime = currFrameTime;

        renderer->begin();

        while(auto keyEvent = surface->getKeyboard().getKeyEvent()) {
            InputEvent const event{ *keyEvent, InputEvent::Type::Keyboard };
            for(auto &&[key, group] : layers) {
                for(auto &layer : group) {
                    if(layer->onInputEvent(event) == InputResponse::Consumed) {
                        break;
                    }
                }
            }
        }
        while(auto mouseEvent = surface->getMouse().getEvent()) {
            InputEvent const event{ *mouseEvent, InputEvent::Type::Mouse };
            for(auto &&[key, group] : layers) {
                for(auto &layer : group) {
                    if(layer->onInputEvent(event) == InputResponse::Consumed) {
                        break;
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
        currentState = State::Stopped;
        graphicsDevice->waitForIdleDevice();
        surface.reset();
    }

    Application::Application() {
        CLOVE_ASSERT(instance == nullptr, "Only one Application can be active");
        instance = this;

        prevFrameTime = std::chrono::system_clock::now();
    }
}