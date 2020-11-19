#pragma once

#include "Stem/LayerStack.hpp"

#include <Clove/Audio/Audio.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <Clove/Platform/PlatformTypes.hpp>
#include <optional>

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

namespace blb::ecs {
    class World;
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
            std::optional<clv::plt::WindowDescriptor> windowDescriptor; /** Not providing a window descriptor will create a headless application. */
            clv::gfx::API graphicsApi{ clv::gfx::API::Vulkan };
            clv::AudioAPI audioApi{ clv::AudioAPI::OpenAl };
        };

        enum class State {
            Running,
            Stopped
        };

        //VARIABLES
    private:
        static Application *instance;

        std::unique_ptr<clv::plt::Platform> platformInstance;
        std::unique_ptr<clv::gfx::GraphicsDevice> graphicsDevice;
        std::unique_ptr<clv::AudioFactory> audioFactory;

        std::shared_ptr<clv::plt::Window> window;
        std::unique_ptr<ForwardRenderer3D> renderer;
        std::unique_ptr<blb::ecs::World> world;

        LayerStack layerStack;

        std::chrono::system_clock::time_point prevFrameTime;

        //FUNCTIONS
    public:
        Application();

        Application(Application const &other)     = delete;
        Application(Application &&other) noexcept = delete;

        Application &operator=(Application const &other) = delete;
        Application &operator=(Application &&other) noexcept = delete;

        ~Application();

        static Application &get();

        State getState() const;

        /**
         * @brief Performs a single iteration of the main application loop.
         * @details getState should be called before calling this to check
         * if the application should still be running.
         */
        void tick();

        /**
         * @brief Transition to State::Stoped if the application is in State::Running.
         */
        void shutdown();

        clv::gfx::GraphicsDevice *getGraphicsDevice() const;
        clv::AudioFactory *getAudioFactory() const;

        std::shared_ptr<clv::plt::Window> const &getWindow() const;
        ForwardRenderer3D *getRenderer() const;
        blb::ecs::World *getECSWorld() const;
    };
}

//To be defined by the client
extern garlic::Application::Descriptor getApplicationDescriptor();
extern std::shared_ptr<garlic::Layer> createApplicationLayer(garlic::Application const &app);
