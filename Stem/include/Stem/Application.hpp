#pragma once

#include "Stem/LayerStack.hpp"

#include <Clove/Platform/PlatformTypes.hpp>

namespace clv::plt {
    class Platform;
    class Window;
}

namespace garlic::inline stem {
    class Layer;
}

namespace garlic::inline stem {
    class Application {
        //TYPES
    public:
        struct Descriptor {
            clv::plt::WindowDescriptor windowDescriptor;
        };

        //VARIABLES
    private:
        static Application *instance;

        std::unique_ptr<clv::plt::Platform> platformInstance;
        std::shared_ptr<clv::plt::Window> window;

        LayerStack layerStack;

        std::chrono::system_clock::time_point prevFrameTime;

        //FUNCTIONS
    public:
        Application();

        Application(const Application &other) = delete;
        Application(Application&& other) noexcept = delete;

        Application &operator=(const Application& other) = delete;
        Application &operator=(Application &&other) noexcept = delete;

        ~Application();

        static Application &get();

        void start();
        void run();

        std::shared_ptr<clv::plt::Window> const &getWindow() const;
    };
}

//To be defined by the client
extern garlic::Application::Descriptor getApplicationDescriptor();
extern std::shared_ptr<garlic::Layer> createApplicationLayer(garlic::Application const &app);
