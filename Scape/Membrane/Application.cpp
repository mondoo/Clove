#define EXPORT

#include "Application.hpp"

#include <Bulb/Layer.hpp>
#include <Stem/Application.hpp>

garlic::Application::Descriptor getApplicationDescriptor() {
    garlic::Application::Descriptor descriptor{};
    descriptor.windowDescriptor.title  = "Garlic";
    descriptor.windowDescriptor.width  = 1280;
    descriptor.windowDescriptor.height = 720;

    return descriptor;
}

std::shared_ptr<blb::Layer> createApplicationLayer(garlic::Application const &app) {
    return std::make_shared<blb::Layer>("Empty layer");
}

garlic::Application *application_create() {
    return new garlic::Application;
}

void application_destroy(garlic::Application *app) {
    delete app;
}

void application_start(garlic::Application *app) {
    app->start();
}

void application_run(garlic::Application *app) {
    app->run();
}