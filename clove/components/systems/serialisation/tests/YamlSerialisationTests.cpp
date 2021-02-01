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

    root["value"] = 5.0f;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\nvalue: 5");
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

TEST(YamlSerialisationTests, CanPushASerialisableStruct) {
    Node root{};

    TestStruct testStruct{
        .memberOne   = 1,
        .memberTwo   = 2,
        .memberThree = 3,
    };

    //TODO: root["testStruct"] = testStruct;
    root["testStruct"].value = serialise(testStruct).value;

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
        //TODO: node["memberTwo"] = object.memberTwo;
        node["memberTwo"].value = serialise(object.memberTwo).value;
        return node;
    }
}

TEST(YamlSerialisationTests, CanPushANestedSerialisableStruct) {
    Node root{};

    NestedTestStruct testStruct{
        .memberOne = 1,
        .memberTwo = {
            .memberOne   = 1,
            .memberTwo   = 2,
            .memberThree = 3,
        },
    };

    //TODO: root["testStruct"] = testStruct;
    root["testStruct"].value = serialise(testStruct).value;

    EXPECT_EQ(emittYaml(root), "type: yaml\nversion: 1\ntestStruct:\n  memberOne: 1\n  memberTwo:\n    memberOne: 1\n    memberTwo: 2\n    memberThree: 3");
}

//TODO: Deserialise