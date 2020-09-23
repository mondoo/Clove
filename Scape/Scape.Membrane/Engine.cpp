#define EXPORT

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

stm::Application* app = nullptr;

void startEngine(){
    app = new stm::Application();
    app->start();
}

void runEngine(){
    app->run();
}