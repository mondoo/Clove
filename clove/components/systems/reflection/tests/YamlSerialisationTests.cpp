
#include <Clove/Reflection/Reflect.hpp>
#include <Clove/Reflection/YamlSerialiser.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

TEST(YamlSerialisationTests, CanEmittDefaultInformation) {
    YamlSerialiser serialiser{};

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1");
}

TEST(YamlSerialisationTests, CanPushANode) {
    YamlSerialiser serialiser{};

    serialiser.push("Start");

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nStart: 0");

    serialiser.push("Node");

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nStart:\n  Node: 0");
}

TEST(YamlSerialisationTests, CanPushANodeWithAValue) {
    YamlSerialiser serialiser{};

    serialiser.push("value", 5.0f);

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nvalue: 5");
}

struct TestClass {
    int32_t memberOne;
    int32_t memberTwo;
    float memberThree;
};
CLOVE_REFLECT_CLASS(
    TestClass,
    CLOVE_REFLECT_CLASS_MEMBER(TestClass, memberOne),
    CLOVE_REFLECT_CLASS_MEMBER(TestClass, memberTwo),
    CLOVE_REFLECT_CLASS_MEMBER(TestClass, memberThree), )

TEST(YamlSerialisationTests, CanPushAReflectedClass) {
    YamlSerialiser serialiser{};

    TestClass testClass{
        .memberOne   = 1,
        .memberTwo   = 2,
        .memberThree = 3.0f,
    };

    serialiser.push("testClass", testClass);

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nTestClass:\n  memberOne: 0\n  memberTwo: 0\n  memberThree: 0");
}