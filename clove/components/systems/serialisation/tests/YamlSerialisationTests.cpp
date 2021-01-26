#include <Clove/Serialisation/YamlSerialiser.hpp>
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

struct TestStruct {
    int32_t memberOne;
    int32_t memberTwo;
    float memberThree;
};

namespace garlic::clove {
    template<>
    Serialiser::Node serialise<TestStruct>(TestStruct const &object) {
        Serialiser::Node node{};
        node["memberOne"]   = object.memberOne;
        node["memberTwo"]   = object.memberTwo;
        node["memberThree"] = object.memberThree;
        return node;
    }

    template<>
    TestStruct deserialise<TestStruct>(Serialiser::Node const &object) {
        return {};
    }
}

TEST(YamlSerialisationTests, CanPushASerialisableStruct) {
    YamlSerialiser serialiser{};

    TestStruct testStruct{
        .memberOne   = 1,
        .memberTwo   = 2,
        .memberThree = 3.0f,
    };

    serialiser.push("testStruct", testStruct);

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\ntestStruct:\n  memberOne: 1\n  memberTwo: 2\n  memberThree: 3");
}

//TODO: Deserialise