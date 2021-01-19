
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

    EXPECT_EQ(serialiser.emitt(), "Start");
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

/* TEST(YamlSerialisationTests, CanConsumeAReflectedClass) {
    YamlSerialiser serialiser{};

    TestClass testClass{
        .memberOne   = 1,
        .memberTwo   = 2,
        .memberThree = 3.0f,
    };

    serialiser.push(testClass, "testClass");

    //EXPECT_TRUE(false);
} */