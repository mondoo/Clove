#include "Stem/Application.hpp"

#include "Stem/InputEvent.hpp"
#include "Stem/Layer.hpp"
#include "Stem/Rendering/ForwardRenderer3D.hpp"

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

    Application::Application() {
        GARLIC_ASSERT(instance == nullptr, "Only one Application can be active");
        instance = this;

        auto const descriptor{ getApplicationDescriptor() };

        //Platform
        platformInstance = clv::plt::createPlatformInstance();
        window           = platformInstance->createWindow(descriptor.windowDescriptor);

        //Graphics
        graphicsDevice = clv::gfx::createGraphicsDevice(descriptor.graphicsApi, window->getNativeWindow());

        //Audio
        audioFactory = clv::createAudioFactory(descriptor.audioApi);

        window->setVSync(true);
        renderer = std::make_unique<ForwardRenderer3D>();
        world    = std::make_unique<blb::ecs::World>();

        layerStack.pushLayer(createApplicationLayer(*this));

        prevFrameTime = std::chrono::system_clock::now();
    }

    Application::~Application() = default;

    Application &Application::get() {
        return *instance;
    }

    Application::State Application::getState() const {
        return window->isOpen() ? State::Running : State::Stopped;
    }

    void Application::tick() {
        auto const currFrameTime{ std::chrono::system_clock::now() };
        std::chrono::duration<float> const deltaSeonds{ currFrameTime - prevFrameTime };
        prevFrameTime = currFrameTime;

        window->processInput();
        renderer->begin();

        //Respond to input
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

        //Do client logic
        for(auto const &layer : layerStack) {
            layer->onUpdate(deltaSeonds.count());
        }

        //Update ECS
        world->update(deltaSeonds.count());

        //Render
        renderer->end();
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
}