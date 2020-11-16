#include "Stem/Application.hpp"

#include "Stem/InputEvent.hpp"
#include "Stem/Layer.hpp"

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
        //...

        window->setVSync(true);

        layerStack.pushLayer(createApplicationLayer(*this));

        prevFrameTime = std::chrono::system_clock::now();
    }

    Application::~Application() = default;

    Application &Application::get() {
        return *instance;
    }

    void Application::run() {
        while(window->isOpen()) {
            auto currFrameTime                       = std::chrono::system_clock::now();
            std::chrono::duration<float> deltaSeonds = currFrameTime - prevFrameTime;
            prevFrameTime                            = currFrameTime;

            window->processInput();

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

            //Do frame logic
            for(auto const &layer : layerStack) {
                layer->onUpdate(deltaSeonds.count());
            }
        }
    }

    std::shared_ptr<clv::plt::Window> const &Application::getWindow() const {
        return window;
    }

    clv::gfx::GraphicsDevice *Application::getGraphicsDevice() const{
        return graphicsDevice.get();
    }
}