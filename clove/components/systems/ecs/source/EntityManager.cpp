#include "Clove/ECS/EntityManager.hpp"

namespace garlic::clove {
    EntityManager::EntityManager()
        : componentManager(&ecsEventDispatcher) {
    }

    EntityManager::~EntityManager() {
        destroyAll();
    }

    Entity EntityManager::create() {
        Entity entity{ nextEntity++ };
        activeEntities.push_back(entity);

        return entity;
    }

    Entity EntityManager::clone(Entity entity) {
        Entity clonedEntity{ create() };
        componentManager.cloneComponents(entity, clonedEntity);

        return clonedEntity;
    }

    void EntityManager::destroy(Entity entity) {
        auto foundIDIter{ std::find(activeEntities.begin(), activeEntities.end(), entity) };

        if(entity == NullEntity || foundIDIter == activeEntities.end()) {
            return;
        }

        pendingDestroyEntities.emplace(entity);
    }

    void EntityManager::destroyAll() {
        for(Entity entity : activeEntities) {
            //componentManager.onEntityDestroyed(entity);
        }
        activeEntities.clear();
    }

    bool EntityManager::isValid(Entity entity) {
        if(entity != NullEntity) {
            return std::find(activeEntities.begin(), activeEntities.end(), entity) != activeEntities.end();
        } else {
            return false;
        }
    }
}
