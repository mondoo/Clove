#pragma once

namespace stm {
    class Application;
}

namespace Scape::Membrane {
    /**
     * @brief Wraps a Stem Application.
     */
public
    ref class Engine {
        //VARIABLES
    private:
        stm::Application* app;

        //FUNCTIONS
    public:
        Engine();

        ~Engine();
        !Engine();

        void start();
        void run();
    };
}