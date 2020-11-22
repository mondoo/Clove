#include "Scape/Membrane/Application.hpp"

using namespace System::Windows::Media;

namespace Garlic {

    Application::Application(int const width, int const height) {
        app = new wrapper::Application(width, height);
        resize(width, height);
    }

    Application::~Application() {
        //delete writeableBitmap;
       
        this->!Application();
    }

    Application::!Application() {
        delete app;
    }

    bool Application::isRunning() {
        return app->isRunning();
    }

    void Application::tick() {
        app->tick();
    }

    void Application::shutdown() {
        app->shutdown();
    }

    void Application::resize(int width, int height) {
        app->resize(width, height);
    }

    void Application::render(System::IntPtr backBuffer){
        app->copyRenderTargetToPointer(backBuffer.ToPointer());
    }
}