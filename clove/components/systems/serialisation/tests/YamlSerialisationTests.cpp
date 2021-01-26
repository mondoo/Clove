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

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nStart: 0\nNode: 0");
}

TEST(YamlSerialisationTests, CanPushANodeWithAValue) {
    YamlSerialiser serialiser{};

    serialiser.push("value", 5.0f);

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nvalue: 5");
}

TEST(YamlSerialisationTests, CanPushACustomNode) {
    YamlSerialiser serialiser{};

    Serialiser::Node node{ "CustomNode" };
    node = 1;
    serialiser.push(std::move(node));

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nCustomNode: 1");

    Serialiser::Node parentNode{ "ParentNode" };
    parentNode["value1"] = 42;
    parentNode["value2"] = 100;

    serialiser.push(std::move(parentNode));

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nCustomNode: 1\nParentNode:\n  value1: 42\n  value2: 100");

    Serialiser::Node otherNode{ "OtherNode" };
    otherNode = 3;

    serialiser.push(std::move(otherNode));

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\nCustomNode: 1\nParentNode:\n  value1: 42\n  value2: 100\nOtherNode: 3");
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
}

TEST(YamlSerialisationTests, CanPushASerialisableStruct) {
    YamlSerialiser serialiser{};

    TestStruct testStruct{
        .memberOne   = 1,
        .memberTwo   = 2,
        .memberThree = 3,
    };

    serialiser.push("testStruct", testStruct);

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\ntestStruct:\n  memberOne: 1\n  memberTwo: 2\n  memberThree: 3");
}

struct NestedTestStruct {
    float memberOne;
    TestStruct memberTwo;
};

namespace garlic::clove {
    template<>
    Serialiser::Node serialise<NestedTestStruct>(NestedTestStruct const &object) {
        Serialiser::Node node{};
        node["memberOne"] = object.memberOne;
        //TODO: node["memberTwo"] = object.memberTwo;
        node["memberTwo"].value = serialise(object.memberTwo).value;
        return node;
    }
}

TEST(YamlSerialisationTests, CanPushANestedSerialisableStruct) {
    YamlSerialiser serialiser{};

    NestedTestStruct testStruct{
        .memberOne = 1,
        .memberTwo = {
            .memberOne   = 1,
            .memberTwo   = 2,
            .memberThree = 3,
        },
    };

    serialiser.push("testStruct", testStruct);

    EXPECT_EQ(serialiser.emitt(), "type: yaml\nversion: 1\ntestStruct:\n  memberOne: 1\n  memberTwo:\n    memberOne: 1\n    memberTwo: 2\n    memberThree: 3");
}

//TODO: Deserialise