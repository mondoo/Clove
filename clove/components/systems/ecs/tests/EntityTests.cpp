#include <Clove/ECS/EntityManager.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

TEST(ECSEntityTests, DefaultInitialiseEntity) {
    Entity entity{};

    ASSERT_EQ(entity, NullEntity);
}

TEST(ECSEntityTests, CreateEntity) {
    EntityManager manager{};
    Entity entity{ manager.create() };

    ASSERT_NE(entity, NullEntity);
}