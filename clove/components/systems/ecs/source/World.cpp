#include "Clove/ECS/World.hpp"

namespace garlic::clove {
    Entity World::nextEntity = 1;

    World::World()
        : componentManager(&ecsEventDispatcher) {
    }

    World::~World() {
        destroyAll();
    }

    Entity World::create() {
        Entity entity{ nextEntity++ };
        activeEntities.push_back(entity);

        return entity;
    }

    Entity World::clone(Entity entity) {
        Entity clonedEntity{ create() };
        componentManager.cloneEntitiesComponents(entity, clonedEntity);

        return clonedEntity;
    }

    void World::destroy(Entity entity) {
        auto foundIDIter{ std::find(activeEntities.begin(), activeEntities.end(), entity) };

        if(entity == NullEntity || foundIDIter == activeEntities.end()) {
            return;
        }

        pendingDestroyEntities.emplace(entity);
    }

    void World::destroyAll() {
        for(Entity entity : activeEntities) {
            componentManager.onEntityDestroyed(entity);
        }
        activeEntities.clear();
    }

    bool World::isValid(Entity entity) {
        if(entity != NullEntity) {
            return std::find(activeEntities.begin(), activeEntities.end(), entity) != activeEntities.end();
        } else {
            return false;
        }
    }
}
