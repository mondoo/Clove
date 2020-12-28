#include <Clove/ECS/EntityManager.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

struct BoolComponent {
    bool value{ false };
};

struct ComplexComponent {
    int32_t a{ 0 };
    int32_t b{ 0 };
    int32_t c{ 0 };
};

struct FloatComponent {
    float value{ 0.0f };
};

TEST(ECSComponentTests, CanAddSingleComponentToEntity) {
    EntityManager manager{};

    Entity defaultEntity{ manager.create() };
    Entity falseEntity{ manager.create() };
    Entity trueEntity{ manager.create() };

    auto &defaultComp{ manager.addComponent<BoolComponent>(defaultEntity) };
    EXPECT_EQ(defaultComp.value, BoolComponent{}.value);

    auto &falseComp{ manager.addComponent<BoolComponent>(falseEntity, false) };
    EXPECT_FALSE(falseComp.value);

    auto &trueComp{ manager.addComponent<BoolComponent>(trueEntity, true) };
    EXPECT_TRUE(trueComp.value);

    Entity complexEntity{ manager.create() };

    auto &complexComp{ manager.addComponent<ComplexComponent>(complexEntity, 1, 2, 3) };
    EXPECT_EQ(complexComp.a, 1);
    EXPECT_EQ(complexComp.b, 2);
    EXPECT_EQ(complexComp.c, 3);

    Entity trueOrFalseEntity{ manager.create() };

    auto &trueOrFalseCompA{ manager.addComponent<BoolComponent>(trueOrFalseEntity, false) };
    EXPECT_FALSE(trueOrFalseCompA.value);

    auto &trueOrFalseCompB{ manager.addComponent<BoolComponent>(trueOrFalseEntity, true) };
    EXPECT_TRUE(trueOrFalseCompA.value);
    EXPECT_TRUE(trueOrFalseCompB.value);

    trueOrFalseCompA.value = false;
    EXPECT_FALSE(trueOrFalseCompA.value);
    EXPECT_FALSE(trueOrFalseCompB.value);

    ASSERT_TRUE(CLOVE_ENABLE_ASSERTIONS);

    EXPECT_DEATH(manager.addComponent<BoolComponent>(NullEntity), "");
    
    Entity badEntity{ 999999999 };
    EXPECT_DEATH(manager.addComponent<BoolComponent>(badEntity), "");
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