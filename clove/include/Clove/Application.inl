namespace garlic::clove {
    Surface *Application::getSurface() const {
        return surface.get();
    }

    GraphicsDevice *Application::getGraphicsDevice() const {
        return graphicsDevice.get();
    }

    AudioDevice *Application::getAudioDevice() const {
        return audioDevice.get();
    }

    ForwardRenderer3D *Application::getRenderer() const {
        return renderer.get();
    }

    EntityManager *Application::getEntityManager() const {
        return entityManager.get();
    }

    PhysicsLayer *Application::getPhysicsLayer() const {
        return physicsLayer.get();
    }
}