#include "Scape/Membrane/Application.hpp"

namespace Garlic{
    Application::Application(){
        app = new wrapper::Application;
    }

    Application::~Application() {
    }

    Application::!Application(){
        delete app;
    }
}