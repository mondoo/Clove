#pragma once

#include "Stem/LayerStack.hpp"

#include <Clove/Audio/Audio.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <Clove/Platform/PlatformTypes.hpp>

namespace clv::plt {
    class Platform;
    class Window;
}

namespace clv::gfx {
    class GraphicsDevice;
}

namespace clv {
    class AudioFactory;
}

namespace garlic::inline stem {
    class Layer;
    class ForwardRenderer3D;
}

namespace garlic::inline stem {
    class Application {
        //TYPES
    public:
        struct Descriptor {
            clv::plt::WindowDescriptor windowDescriptor;
            clv::gfx::API graphicsApi{ clv::gfx::API::Vulkan };
            clv::AudioAPI audioApi{ clv::AudioAPI::OpenAl };
        };

        //VARIABLES
    private:
        static Application *instance;

        std::unique_ptr<clv::plt::Platform> platformInstance;
        std::unique_ptr<clv::gfx::GraphicsDevice> graphicsDevice;
        std::unique_ptr<clv::AudioFactory> audioFactory;

        std::shared_ptr<clv::plt::Window> window;
        std::unique_ptr<ForwardRenderer3D> renderer;

        LayerStack layerStack;

        std::chrono::system_clock::time_point prevFrameTime;

        //FUNCTIONS
    public:
        Application();

        Application(const Application &other)     = delete;
        Application(Application &&other) noexcept = delete;

        Application &operator=(const Application &other) = delete;
        Application &operator=(Application &&other) noexcept = delete;

        ~Application();

        static Application &get();

        /**
         * @brief Runs the main application loop until told to exit.
         */
        void run();

        clv::gfx::GraphicsDevice *getGraphicsDevice() const;
        clv::AudioFactory *getAudioFactory() const;

        std::shared_ptr<clv::plt::Window> const &getWindow() const;
        ForwardRenderer3D *getRenderer() const;
    };
}

//To be defined by the client
extern garlic::Application::Descriptor getApplicationDescriptor();
extern std::shared_ptr<garlic::Layer> createApplicationLayer(garlic::Application const &app);
