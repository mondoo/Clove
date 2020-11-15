#pragma once

#include <Bulb/LayerStack.hpp>
#include <Clove/Platform/PlatformTypes.hpp>

namespace clv::plt {
    class Platform;
    class Window;
}

namespace blb {
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
        std::unique_ptr<clv::plt::Platform> platformInstance;
        std::shared_ptr<clv::plt::Window> window;

        blb::LayerStack layerStack;

        std::chrono::system_clock::time_point prevFrameTime;

        //FUNCTIONS
    public:
        Application();
        ~Application();

        void start();
        void run();

        std::shared_ptr<clv::plt::Window> const &getWindow() const;
    };
}

//To be defined by the client
extern garlic::Application::Descriptor getApplicationDescriptor();
extern std::shared_ptr<blb::Layer> createApplicationLayer(garlic::Application const &app);
