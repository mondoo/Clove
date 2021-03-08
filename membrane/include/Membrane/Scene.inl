namespace garlic::membrane {
    template<typename ComponentType, typename... ConstructArgs>
    ComponentType &Scene::addComponent(clove::Entity entity, ConstructArgs &&...args) {
        //TODO: Track components?
        return manager->addComponent<ComponentType>(entity, std::forward<ConstructArgs>(args)...);
    }
}