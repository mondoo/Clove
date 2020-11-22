#pragma once

#include "Stem/LayerStack.hpp"

#include <Clove/Audio/Audio.hpp>
#include <Clove/Graphics/GraphicsAPI.hpp>
#include <Clove/Platform/PlatformTypes.hpp>
#include <Clove/Graphics/GraphicsImage.hpp>
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
    class GraphicsImageRenderTarget;
}

namespace garlic::inline stem {
    class Application {
        //TYPES
    public:
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
        //Application() = delete;

        Application(Application const &other)     = delete;
        Application(Application &&other) noexcept = delete;

        Application &operator=(Application const &other) = delete;
        Application &operator=(Application &&other) noexcept = delete;

        ~Application();

        /**
         * @brief Creates a standard Garlic application that opens and manages it's own window
         */
        friend std::unique_ptr<Application> createApplication(clv::gfx::API graphicsApi, clv::AudioAPI audioApi, clv::plt::WindowDescriptor windowDescriptor);

        /**
         * @brief Create a headles Garlic application without a window.
         */
        friend std::pair<std::unique_ptr<Application>, GraphicsImageRenderTarget*> createHeadlessApplication(clv::gfx::API graphicsApi, clv::AudioAPI audioApi, clv::gfx::GraphicsImage::Descriptor renderTargetDescriptor);

        static Application &get();

        void pushLayer(std::shared_ptr<Layer> layer);

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

    private:
        Application();
    };
}