namespace garlic::clove{
    template<typename ComponentType, typename... ConstructArgs>
    ComponentType &World::addComponent(Entity entity, ConstructArgs &&...args) {
        //TODO: Track components?
        return manager->addComponent<ComponentType>(entity, std::forward<ConstructArgs>(args)...);
    }
}