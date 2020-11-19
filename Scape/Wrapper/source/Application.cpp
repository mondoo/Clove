#include "Scape/Wrapper/Application.hpp"

#include <Stem/Application.hpp>
#include <Stem/Layer.hpp>

garlic::Application::Descriptor getApplicationDescriptor(){
    return garlic::Application::Descriptor{
        .windowDescriptor = {
            .title  = "Temp",
            .width  = 800,
            .height = 600,
        },
    };
}

std::shared_ptr<garlic::Layer> createApplicationLayer(garlic::Application const &app){
    return std::make_shared<garlic::Layer>("Empty layer");
}

struct AppWrapper {
    garlic::Application app;
};

namespace wrapper {
    Application::Application(){
        appWrapper = std::make_unique<AppWrapper>();
    }

    Application::~Application() = default;
}