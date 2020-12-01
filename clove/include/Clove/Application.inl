namespace garlic::clove{
    GraphicsDevice *Application::getGraphicsDevice() const {
        return graphicsDevice.get();
    }

    AudioDevice *Application::getAudioDevice() const {
        return audioDevice.get();
    }

    Window *Application::getWindow() const {
        return window.get();
    }

    ForwardRenderer3D *Application::getRenderer() const {
        return renderer.get();
    }

    World *Application::getECSWorld() const {
        return world.get();
    }
}