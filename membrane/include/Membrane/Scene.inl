namespace garlic::membrane {
    std::vector<clove::Entity> Scene::getKnownEntities() const {
        return knownEntities;
    }

    template<typename ComponentType, typename... ConstructArgs>
    ComponentType &Scene::addComponent(clove::Entity entity, ConstructArgs &&... args) {
        return manager->addComponent<ComponentType>(entity, std::forward<ConstructArgs>(args)...);
    }
}