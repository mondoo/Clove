#include <Clove/ECS/EntityManager.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

struct BoolComponent {
    bool value{ false };
};

struct FloatComponent {
    float value{ 0.0f };
};

TEST(ECSComponentTests, CanAddSingleComponentToEntity) {
    EntityManager manager{};

    {
        Entity defaultEntity{ manager.create() };
        Entity valueEntity{ manager.create() };

        auto &falseComp{ manager.addComponent<BoolComponent>(defaultEntity) };
        auto &trueComp{ manager.addComponent<BoolComponent>(valueEntity, true) };

        ASSERT_FALSE(falseComp.value);
        ASSERT_TRUE(trueComp.value);
    }

    {
        Entity trueOrFalseEntity{ manager.create() };
        auto &trueOrFalseComp{ manager.addComponent<BoolComponent>(trueOrFalseEntity) };

        ASSERT_FALSE(trueOrFalseComp.value);

        auto &trueComp{ manager.addComponent<BoolComponent>(trueOrFalseEntity, true) };

        ASSERT_TRUE(trueOrFalseComp.value);
        ASSERT_TRUE(trueComp.value);

        trueOrFalseComp.value = false;

        ASSERT_FALSE(trueOrFalseComp.value);
        ASSERT_FALSE(trueComp.value);
    }

    //TODO: Tests for adding a component to an entity that already exists
    //TODO: Tests for adding a component to an entity that doesn't exist
}

// TEST(ECSComponentTests, CanCheckIfAnEntityHasAComponent) {
//     EntityManager manager{};

//     Entity entity{ manager.create() };
//     manager.addComponent<BoolComponent>(entity);

//     ASSERT_TRUE(manager.hasComponent<BoolComponent>(entity));
//     ASSERT_FALSE(manager.hasComponent<FloatComponent>(entity));
// }

// TEST(ECSComponentTests, CanGetAnEntitysComponent) {
//     //TODO Make sure to assert_death for ones that do not have component type.
// }

// TEST(ECSComponentTests, CanAddMultipleComponentsToEntity) {
//     EntityManager manager{};

//     Entity entity{ manager.create() };
//     manager.addComponent<BoolComponent>(entity);
//     manager.addComponent<FloatComponent>(entity);

//     ASSERT_TRUE(manager.hasComponent<BoolComponent>(entity));
//     ASSERT_TRUE(manager.hasComponent<FloatComponent>(entity));
// }