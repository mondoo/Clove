namespace garlic::clove{
    GraphicsDevice *Application::getGraphicsDevice() const {
        return graphicsDevice.get();
    }

    AudioDevice *Application::getAudioDevice() const {
        return audioDevice.get();
    }

    std::shared_ptr<Window> const &Application::getWindow() const {
        return window;
    }

    ForwardRenderer3D *Application::getRenderer() const {
        return renderer.get();
    }

    World *Application::getECSWorld() const {
        return world.get();
    }
}