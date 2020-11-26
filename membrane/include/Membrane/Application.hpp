#pragma once

#include <Clove/Application.hpp>

namespace garlic::clove {
    class GraphicsImageRenderTarget;
}

namespace garlic::membrane{
    class EditorLayer;
}

namespace garlic::membrane {
public ref class Application {
    private:
        clove::Application *app;
        clove::GraphicsImageRenderTarget *renderTarget;

        std::shared_ptr<EditorLayer> *layer;

        int width;
        int height;

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