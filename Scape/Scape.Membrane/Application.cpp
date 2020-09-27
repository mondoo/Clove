#define EXPORT

#include "Application.hpp"

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

stm::Application* application_create(){
    return new stm::Application;
}

void application_destroy(stm::Application* app){
    delete app;
}

void application_start(stm::Application* app){
    app->start();
}

void application_run(stm::Application* app){
    app->run();
}