#include "Scape/Membrane/Application.hpp"

using namespace System::Windows::Media;

namespace Garlic {

    Application::Application(int const width, int const height) {
        resize(width, height);

        app = new wrapper::Application(width, height);
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

    void Application::shutdown() {
        app->shutdown();
    }

    void Application::resize(int width, int height) {
        //writeableBitmap can't be initialised with 0,0 
        if(width <= 0){
            width = 1;
        }
        if(height <= 0 ){
            height = 1;
        }
        //Hard coding format to B8G8R8A8_SRGB
        writeableBitmap = gcnew Imaging::WriteableBitmap(width, height, 96, 96, PixelFormats::Pbgra32, nullptr);
    }
}