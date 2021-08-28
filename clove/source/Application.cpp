#include "Clove/Application.hpp"

#include "Clove/InputEvent.hpp"
#include "Clove/Rendering/ForwardRenderer3D.hpp"
#include "Clove/Rendering/GraphicsImageRenderTarget.hpp"
#include "Clove/Rendering/SwapchainRenderTarget.hpp"
#include "Clove/SubSystem.hpp"
#include "Clove/SubSystems/AudioSubSystem.hpp"
#include "Clove/SubSystems/PhysicsSubSystem.hpp"
#include "Clove/SubSystems/RenderSubSystem.hpp"
#include "Clove/SubSystems/TransformSubSystem.hpp"
#include "Clove/WindowSurface.hpp"

#include <Clove/Audio/AhaDevice.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Graphics/GhaDevice.hpp>
#include <Clove/Graphics/Graphics.hpp>
#include <Clove/Log/Log.hpp>

namespace clove {
    Application *Application::instance{ nullptr };

    Application::~Application() {
        //Tell all layers they have been detached when the application is shutdown
        for(auto &&[key, group] : subSystems) {
            for(auto &layer : group) {
                layer->onDetach();
            }
        }
    }

    std::unique_ptr<Application> Application::create(GraphicsApi graphicsApi, AudioApi audioApi, Window::Descriptor const &windowDescriptor) {	
		auto window{ Window::create(windowDescriptor) };
		auto *windowPtr{ window.get() };
		
		auto graphicsDevice{ createGraphicsDevice(graphicsApi, window->getNativeWindow()) };
		auto audioDevice{ createAudioDevice(audioApi) };
		
		auto surface{ std::make_unique<WindowSurface>(std::move(window)) };
		
		auto renderTarget{ std::make_unique<SwapchainRenderTarget>(*surface, graphicsDevice.get()) };
		
		std::unique_ptr<Application> app{ new Application{ std::move(graphicsDevice), std::move(audioDevice), std::move(surface), std::move(renderTarget) } };
		windowPtr->onWindowCloseDelegate.bind(&Application::shutdown, app.get());
		
        return app;
    }

    std::pair<std::unique_ptr<Application>, GraphicsImageRenderTarget *> Application::createHeadless(GraphicsApi graphicsApi, AudioApi audioApi, GhaImage::Descriptor renderTargetDescriptor, std::unique_ptr<Surface> surface) {
        auto graphicsDevice{ createGraphicsDevice(graphicsApi, std::any{}) };
        auto audioDevice{ createAudioDevice(audioApi) };

        auto renderTarget{ std::make_unique<GraphicsImageRenderTarget>(renderTargetDescriptor, graphicsDevice->getGraphicsFactory()) };
        auto *renderTargetPtr{ renderTarget.get() };

        std::unique_ptr<Application> app{ new Application{ std::move(graphicsDevice), std::move(audioDevice), std::move(surface), std::move(renderTarget) } };

        return { std::move(app), renderTargetPtr };
    }

    Application &Application::get() {
        return *instance;
    }

    void Application::pushSubSystem(std::shared_ptr<SubSystem> layer, LayerGroup group) {
        CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Attached layer: {0}", layer->getName());
        layer->onAttach();
        subSystems[group].push_back(std::move(layer));
    }

    void Application::popSubSystem(std::shared_ptr<SubSystem> const &layer) {
        for(auto &&[key, group] : subSystems) {
            if(auto it = std::find(group.begin(), group.end(), layer); it != group.end()) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Popped layer: {0}", (*it)->getName());
                (*it)->onDetach();
                group.erase(it);
                break;
            }
        }
    }

    void Application::tick() {
        surface->processInput();

        //If the previous processInput call closed the window we don't want to run the rest of the function.
        if(currentState != State::Running) {
            return;
        }

        auto const currFrameTime{ std::chrono::system_clock::now() };
        std::chrono::duration<float> const deltaSeonds{ currFrameTime - prevFrameTime };
        prevFrameTime = currFrameTime;

        renderer->begin();

        while(auto keyEvent = surface->getKeyboard().getKeyEvent()) {
            InputEvent const event{ *keyEvent, InputEvent::Type::Keyboard };
            for(auto &&[key, group] : subSystems) {
                for(auto &layer : group) {
                    if(layer->onInputEvent(event) == InputResponse::Consumed) {
                        break;
                    }
                }
            }
        }
        while(auto mouseEvent = surface->getMouse().getEvent()) {
            InputEvent const event{ *mouseEvent, InputEvent::Type::Mouse };
            for(auto &&[key, group] : subSystems) {
                for(auto &layer : group) {
                    if(layer->onInputEvent(event) == InputResponse::Consumed) {
                        break;
                    }
                }
            }
        }

        for(auto &&[key, group] : subSystems) {
            for(auto &layer : group) {
                layer->onUpdate(deltaSeonds.count());
            }
        }

        renderer->end();
    }

    void Application::shutdown() {
        currentState = State::Stopped;
    }

    Application::Application(std::unique_ptr<GhaDevice> graphicsDevice, std::unique_ptr<AhaDevice> audioDevice, std::unique_ptr<Surface> surface, std::unique_ptr<RenderTarget> renderTarget)
        : graphicsDevice{ std::move(graphicsDevice) }
        , audioDevice{ std::move(audioDevice) }
        , surface{ std::move(surface) }
        , assetManager{ &fileSystem } {
        CLOVE_ASSERT(instance == nullptr, "Only one Application can be active");
        instance = this;

        prevFrameTime = std::chrono::system_clock::now();

        //Systems
        renderer = std::make_unique<ForwardRenderer3D>(this->graphicsDevice.get(), std::move(renderTarget));

        //Layers
        physicsSubSystem = std::make_shared<PhysicsSubSystem>(&entityManager);

        pushSubSystem(std::make_shared<TransformSubSystem>(&entityManager), LayerGroup::Initialisation);
        pushSubSystem(physicsSubSystem, LayerGroup::Initialisation);
        pushSubSystem(std::make_shared<AudioSubSystem>(&entityManager), LayerGroup::Render);
        pushSubSystem(std::make_shared<RenderSubSystem>(renderer.get(), &entityManager), LayerGroup::Render);
    }
}
