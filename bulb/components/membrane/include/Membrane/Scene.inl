namespace membrane {
    clove::Entity Scene::createEntity() {
        clove::Entity entity{ manager->create() };
        knownEntities.push_back(entity);
        return entity;
    }

    void Scene::deleteEntity(clove::Entity entity){
        manager->destroy(entity);
        knownEntities.erase(std::remove(knownEntities.begin(), knownEntities.end(), entity), knownEntities.end());
    }

    std::vector<clove::Entity> Scene::getKnownEntities() const {
        return knownEntities;
    }

    clove::EntityManager &Scene::getEntityManager() const {
        return *manager;
    }
}