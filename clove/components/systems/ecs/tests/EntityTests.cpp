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

    Entity entityA{ manager.create() };
    Entity entityB{ manager.create() };
    Entity entityC{ manager.create() };

    EXPECT_NE(entityA, entityB);
    EXPECT_NE(entityA, entityC);
    EXPECT_NE(entityB, entityC);
}