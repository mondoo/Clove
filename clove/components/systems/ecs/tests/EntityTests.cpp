#include <Clove/ECS/EntityManager.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

TEST(ECSEntityTests, DefaultInitialiseEntity) {
    Entity entity{};

    EXPECT_EQ(entity, NullEntity);
}

TEST(ECSEntityTests, CreateEntity) {
    EntityManager manager{};
    Entity entity{ manager.create() };

    EXPECT_NE(entity, NullEntity);

    std::vector<Entity> entities{};
    for(int i{ 0 }; i < 10000; ++i) {
        Entity entity{ manager.create() };

        entities.push_back(entity);

        for(int j{ i - 1 }; j >= 0; --j) {
            EXPECT_NE(entity, entities[j]);
        }
    }
}

//TODO: isValid
//TODO: clone
//TODO: destroy
//TODO: destroyAll
//TODO: Create an entity in one manager and access it in another