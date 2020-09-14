#include "Engine.hpp"

#include <Stem/Application.hpp>
#include <Bulb/Layer.hpp>

stm::ApplicationDescriptor getApplicationDescriptor() {
    stm::ApplicationDescriptor descriptor{};
    descriptor.windowDescriptor.title  = "Garlic";
    descriptor.windowDescriptor.width  = 1280;
    descriptor.windowDescriptor.height = 720;

    return descriptor;
}

std::shared_ptr<blb::Layer> createApplicationLayer(const stm::Application& app) {
    return std::make_shared<blb::Layer>("Empty layer");
}

namespace Scape::Membrane {
    Engine::Engine(){
        app = new stm::Application;
    }

    Engine::~Engine(){
        this->!Engine();
    }

    Engine::!Engine() {
        delete app;
    }
    void Engine::start() {
        app->start();
    }
    void Engine::run() {
        app->run();
    }
}