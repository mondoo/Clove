
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
    float memberOne;
    float memberTwo;
    float memberThree;
};

TEST(YamlSerialisationTests, CanPushAReflectedClass) {
    YamlSerialiser serialiser{};

    TestClass testClass{
        .memberOne   = 1,
        .memberTwo   = 2,
        .memberThree = 3.0f,
    };

    serialiser.push("testClass", testClass);

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nTestClass:\n  memberOne: 1\n  memberTwo: 2\n  memberThree: 3");
}