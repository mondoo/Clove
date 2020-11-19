#include "Scape/Membrane/Application.hpp"

namespace Garlic{
    Application::Application(){
        app = new wrapper::Application;
    }

    Application::~Application() {
        this->!Application();
    }

    Application::!Application(){
        delete app;
    }

    void Application::shutdown(){
        app->shutdown();
    }

    bool Application::tick() {
        return app->tick();
    }
}