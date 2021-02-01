#include <Clove/Serialisation/Node.hpp>
#include <Clove/Serialisation/Yaml.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;
using namespace garlic::clove::serialiser;

TEST(YamlSerialisationTests, CanEmittDefaultInformation) {
    Node node{};

    EXPECT_EQ(emittYaml(node), "type: yaml\nversion: 1");
}

TEST(YamlSerialisationTests, CanAddAChildNode) {
    Node root{};

    root["Start"] = 0;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\nStart: 0");

    root["Node"] = 0;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\nStart: 0\nNode: 0");
}

TEST(YamlSerialisationTests, CanAddAChildNodeWithAValue) {
    Node root{};

    float f{ 99.0f };
    int32_t i{ 100 };
    uint32_t ui{ 200 };
    int16_t i16{ 16 };
    uint16_t ui16{ 1600 };

    root["value1"]   = 5.0f;
    root["value2"]   = 3;
    root["float"]    = f;
    root["int32_t"]  = i;
    root["uint32_t"] = ui;
    root["int16_t"]  = i16;
    root["uint16_t"] = ui16;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\nvalue1: 5\nvalue2: 3\nfloat: 99\nint32_t: 100\nuint32_t: 200\nint16_t: 16\nuint16_t: 1600");
}

TEST(YamlSerialisationTests, CanAddANodeAsAChildNode) {
    Node root{};

    Node parentNode{};
    parentNode["value1"] = 42;
    parentNode["value2"] = 100;

    //TODO: root["ParentNode"] = parentNode;
    root["ParentNode"].value = parentNode.value;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\nParentNode:\n  value1: 42\n  value2: 100");

    root["OtherNode"] = 3;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\nParentNode:\n  value1: 42\n  value2: 100\nOtherNode: 3");
}

struct TestStruct {
    int32_t memberOne;
    int32_t memberTwo;
    float memberThree;
};

namespace garlic::clove {
    template<>
    Node serialise<TestStruct>(TestStruct const &object) {
        Node node{};
        node["memberOne"]   = object.memberOne;
        node["memberTwo"]   = object.memberTwo;
        node["memberThree"] = object.memberThree;
        return node;
    }
}

TEST(YamlSerialisationTests, CanAddASerialisableStruct) {
    Node root{};

    TestStruct testStruct{
        .memberOne   = 1,
        .memberTwo   = 2,
        .memberThree = 3,
    };

    root["testStruct"] = testStruct;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\ntestStruct:\n  memberOne: 1\n  memberTwo: 2\n  memberThree: 3");
}

struct NestedTestStruct {
    float memberOne;
    TestStruct memberTwo;
};

namespace garlic::clove {
    template<>
    Node serialise<NestedTestStruct>(NestedTestStruct const &object) {
        Node node{};
        node["memberOne"] = object.memberOne;
        node["memberTwo"] = object.memberTwo;
        return node;
    }
}

TEST(YamlSerialisationTests, CanAddANestedSerialisableStruct) {
    Node root{};

    NestedTestStruct testStruct{
        .memberOne = 1,
        .memberTwo = {
            .memberOne   = 1,
            .memberTwo   = 2,
            .memberThree = 3,
        },
    };

    root["testStruct"] = testStruct;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\ntestStruct:\n  memberOne: 1\n  memberTwo:\n    memberOne: 1\n    memberTwo: 2\n    memberThree: 3");
}

//TODO: Deserialise