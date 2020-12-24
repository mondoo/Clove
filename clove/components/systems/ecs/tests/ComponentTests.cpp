#include <Clove/ECS/EntityManager.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

struct BoolComponent {
    bool value{ false };
};

TEST(ECSComponentTests, CanAddSingleComponentToEntity) {
    EntityManager manager{};

    Entity defaultEntity{ manager.create() };
    Entity valueEntity{ manager.create() };

    auto &falseComp{ manager.addComponent<BoolComponent>(defaultEntity) };
    auto &trueComp{ manager.addComponent<BoolComponent>(defaultEntity, true) };

    ASSERT_FALSE(falseComp.value);
    ASSERT_TRUE(trueComp.value);
}