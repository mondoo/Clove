#include <Clove/ECS/Component.hpp>
#include <Clove/ECS/World.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

class ValueComponent : public Component<ValueComponent> {
public:
    int32_t value{ 0 };
};

class BoolComponent : public Component<BoolComponent> {
public:
    bool value{ false };
};

void makeTrue(BoolComponent &comp) {
    comp.value = true;
}

TEST(ECSSystemTests, CanUseMemberFunction) {
    World world;

    std::vector<Entity> entities;
    for(int i = 0; i < 5; ++i) {
        auto entity{ world.create() };
        world.addComponent<ValueComponent>(entity);
        world.addComponent<BoolComponent>(entity);
        entities.push_back(entity);
    }

    class TestSystem{
        public:
            int32_t const entityValue{ 18 };

        void update(ValueComponent& component){
            component.value = entityValue;
        }
    };

    TestSystem system;

    world.registerSystem(&TestSystem::update, &system);

    world.update(0.0f);

    for(auto entity : entities){
        EXPECT_EQ(world.getComponent<ValueComponent>(entity)->value, system.entityValue);
    }
}

TEST(ECSSystemTests, CanUseFreeFunction){
    World world;

    std::vector<Entity> entities;
    for(int i = 0; i < 5; ++i) {
        auto entity{ world.create() };
        world.addComponent<ValueComponent>(entity);
        world.addComponent<BoolComponent>(entity);
        entities.push_back(entity);
    }

    world.registerSystem(&makeTrue);

    world.update(0.0f);

    for(auto entity : entities) {
        EXPECT_TRUE(world.getComponent<BoolComponent>(entity)->value);
    }
}