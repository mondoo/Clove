#include <Clove/Serialisation/Node.hpp>
#include <Clove/Serialisation/Yaml.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;
using namespace garlic::clove::serialiser;

//TODO: Error tests

TEST(YamlDeserialisationTests, CanLoadSimpleValueFromFile){
    Node file{ loadYaml(FILE_DIR "/TestFile.yaml") };

    EXPECT_EQ(file["IntValue"].as<int32_t>(), 3);
    EXPECT_EQ(file["FloatValue"].as<float>(), 4.5f);
}

TEST(YamlDeserialisationTests, CanLoadNestedValuesFromParentNodes){
    Node file{ loadYaml(FILE_DIR "/TestFile.yaml") };

    EXPECT_EQ(file["Parent"]["ChildOne"].as<int32_t>(), 1);
    EXPECT_EQ(file["Parent"]["ChildTwo"]["Value"].as<int32_t>(), 8);
}

struct BasicSerialisableType {
    int32_t memberOne;
    float memberTwo;
};

namespace garlic::clove {
    template<>
    Node serialise<BasicSerialisableType>(BasicSerialisableType const &object) {
        Node node{};
        node["memberOne"] = object.memberOne;
        node["memberTwo"] = object.memberTwo;
        return node;
    }

    template<>
    BasicSerialisableType deserialise(Node const &node) {
        return BasicSerialisableType{
            .memberOne = node["memberOne"].as<int32_t>(),
            .memberTwo = node["memberTwo"].as<float>(),
        };
    }
}

TEST(YamlDeserialisationTests, CanLoadSerialisableTypeFromFile){
    Node file{ loadYaml(FILE_DIR "/TestFile.yaml") };

    auto type{ file["TestType"].as<BasicSerialisableType>() };

    EXPECT_EQ(type.memberOne, 420);
    EXPECT_EQ(type.memberTwo, 1.5f);
}


//TODO: Parent types