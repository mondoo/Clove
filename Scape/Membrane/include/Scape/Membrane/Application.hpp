#pragma once

#include <Scape/Wrapper/Application.hpp>

namespace Garlic {
    /*
    Currently just a thin as possible wrapper around stem::Application.
    Eventually only view logic will be in C# and any other logic required
    to communicate with stem will be in here.
    */
public ref class Application {
    private:
        wrapper::Application *app;

        //The bitmap image that will back our image in WPF
        //System::Windows::Media::Imaging::WriteableBitmap ^ writeableBitmap;

    public:
        Application(int const width, int const height);
        ~Application();
        !Application();

        bool isRunning();
        void tick();
        void shutdown();

        void resize(int width, int height);

        void render(System::IntPtr backBuffer);
    };
}