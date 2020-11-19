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

    bool Application::isRunning(){
        return app->isRunning();
    }

    void Application::tick() {
        app->tick();
    }

    void Application::shutdown(){
        app->shutdown();
    }
}