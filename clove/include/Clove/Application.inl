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

    EntityManager *Application::getEntityManager() const {
        return entityManager.get();
    }

    PhysicsLayer *Application::getPhysicsLayer() const{
        return physicsLayer.get();
    }
}