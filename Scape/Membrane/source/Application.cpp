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

        writeableBitmap->Lock();

        app->copyRenderTargetToPointer(writeableBitmap->BackBuffer.ToPointer());
        writeableBitmap->AddDirtyRect(System::Windows::Int32Rect(0, 0, writeableBitmap->Width, writeableBitmap->Height));

        writeableBitmap->Unlock();
    }

    void Application::shutdown() {
        app->shutdown();
    }

    void Application::resize(int width, int height) {
        //Hard coding format to B8G8R8A8_SRGB
        writeableBitmap = gcnew Imaging::WriteableBitmap(width, height, 96, 96, PixelFormats::Pbgra32, nullptr);

        app->resize(width, height);
    }
}