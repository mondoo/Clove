#include <Clove/Blackboard.hpp>

#include <gtest/gtest.h>

using namespace clove;

//TODO: Tests

TEST(BlackboardTests, CanCopyBlackboard) {
    Blackboard a{};
    Blackboard b{};

    Blackboard::Key constexpr intKey{ 0 };
    Blackboard::Key constexpr floatKey{ 1 };

    int constexpr intVal{ 32 };
    float constexpr floatVal{ 32.5 };

    a.setValue(intKey, intVal);
    a.setValue(floatKey, floatVal);

    b = a;
    Blackboard c{ a };

    ASSERT_TRUE(b.hasValue(intKey));
    ASSERT_TRUE(b.hasValue(floatKey));

    ASSERT_TRUE(c.hasValue(intKey));
    ASSERT_TRUE(c.hasValue(floatKey));

    EXPECT_EQ(a.getValue<int>(intKey).value(), b.getValue<int>(intKey).value());
    EXPECT_EQ(a.getValue<int>(intKey).value(), c.getValue<int>(intKey).value());

    EXPECT_EQ(a.getValue<float>(floatKey).value(), b.getValue<float>(floatKey).value());
    EXPECT_EQ(a.getValue<float>(floatKey).value(), c.getValue<float>(floatKey).value());

    b.setValue(intKey, 1);
    c.setValue(intKey, 2);

    EXPECT_NE(a.getValue<int>(intKey).value(), b.getValue<int>(intKey).value());
    EXPECT_NE(a.getValue<int>(intKey).value(), c.getValue<int>(intKey).value());
}