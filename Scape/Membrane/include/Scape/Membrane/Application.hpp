#pragma once

#include <Scape/Wrapper/Application.hpp>

namespace Garlic {
public ref class Application {
    private:
        wrapper::Application *app;

    public:
        Application();
        ~Application();
        !Application();

        bool isRunning();
        void tick();
        void shutdown();
    };
}