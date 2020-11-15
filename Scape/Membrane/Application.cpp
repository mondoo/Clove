#define EXPORT

#include "Application.hpp"

#include <Stem/Application.hpp>
#include <Stem/Layer.hpp>

using namespace garlic;

Application::Descriptor getApplicationDescriptor() {
    return Application::Descriptor{
        .windowDescriptor = {
            .title  = "Garlic",
            .width  = 1280,
            .height = 720,
        }
    };
}

std::shared_ptr<Layer> createApplicationLayer(Application const &app) {
    return std::make_shared<Layer>("Empty layer");
}

Application *application_create() {
    return new Application;
}

void application_destroy(Application *app) {
    delete app;
}

void application_start(Application *app) {
    app->start();
}

void application_run(Application *app) {
    app->run();
}