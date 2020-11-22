#pragma once

#include <memory>

struct AppWrapper;
struct LayerWrapper;

namespace wrapper {
    class Application {
    private:
        std::unique_ptr<AppWrapper> appWrapper;
        std::unique_ptr<LayerWrapper> layerWrapper;

        int width;
        int height;

    public:
        Application(int const width, int const height);
        ~Application();

        bool isRunning();
        void tick();
        void shutdown();

        void resize(int const width, int const height);

        void copyRenderTargetToPointer(void *ptr);
    };
}