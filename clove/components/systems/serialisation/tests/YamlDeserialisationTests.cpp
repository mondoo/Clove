#include <Clove/Serialisation/Node.hpp>
#include <Clove/Serialisation/Yaml.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;
using namespace garlic::clove::serialiser;

TEST(YamlDeserialisationTests, CanLoadSimpleValueFromFile) {
    Node file{ *loadYaml(FILE_DIR "/TestFile.yaml") };

    EXPECT_EQ(file["IntValue"].as<int32_t>(), 3);
    EXPECT_EQ(file["FloatValue"].as<float>(), 4.5f);
}

TEST(YamlDeserialisationTests, CanLoadNestedValuesFromParentNodes) {
    Node file{ *loadYaml(FILE_DIR "/TestFile.yaml") };

    EXPECT_EQ(file["Parent"]["ChildOne"].as<int32_t>(), 1);
    EXPECT_EQ(file["Parent"]["ChildTwo"]["Value"].as<int32_t>(), 8);
}

TEST(YamlDeserialisationTests, CannotGetIncorrectType) {
    Node file{ *loadYaml(FILE_DIR "/TestFile.yaml") };

    EXPECT_ANY_THROW(file["IntValue"].as<std::string>());
}

TEST(YamlDeserialisationTests, ErrorsWhenOpeningNoFile) {
    Expected<Node, LoadError> file{ loadYaml(FILE_DIR "/ThisShouldNeverExist.yaml") };

    ASSERT_FALSE(file.hasValue());
    EXPECT_EQ(file.getError(), LoadError::BadFile);
}

TEST(YamlDeserialisationTests, CannotLoadIncorrectSerialisedType) {
    Expected<Node, LoadError> file{ loadYaml(FILE_DIR "/WrongType.yaml") };

    ASSERT_FALSE(file.hasValue());
    EXPECT_EQ(file.getError(), LoadError::WrongType);
}

TEST(YamlDeserialisationTests, CannotLoadIncorrectSerialisedVersion) {
    Expected<Node, LoadError> file{ loadYaml(FILE_DIR "/WrongVersion.yaml") };

    ASSERT_FALSE(file.hasValue());
    EXPECT_EQ(file.getError(), LoadError::WrongVersion);
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

TEST(YamlDeserialisationTests, CanLoadSerialisableTypeFromFile) {
    Node file{ *loadYaml(FILE_DIR "/TestFile.yaml") };

    auto type{ file["TestType"].as<BasicSerialisableType>() };

    EXPECT_EQ(type.memberOne, 420);
    EXPECT_EQ(type.memberTwo, 1.5f);
}

struct ParentSerialisableType {
    BasicSerialisableType memberOne;
    int32_t memberTwo;
};

namespace garlic::clove {
    template<>
    Node serialise<ParentSerialisableType>(ParentSerialisableType const &object) {
        Node node{};
        node["memberOne"] = object.memberOne;
        node["memberTwo"] = object.memberTwo;
        return node;
    }

    template<>
    ParentSerialisableType deserialise(Node const &node) {
        return ParentSerialisableType{
            .memberOne = node["memberOne"].as<BasicSerialisableType>(),
            .memberTwo = node["memberTwo"].as<int32_t>(),
        };
    }
}

TEST(YamlDeserialisationTests, CanLoadSerialisableParentTypeFromFile) {
    Node file{ *loadYaml(FILE_DIR "/TestFile.yaml") };

    auto type{ file["TestParentType"].as<ParentSerialisableType>() };

    EXPECT_EQ(type.memberOne.memberOne, 100);
    EXPECT_EQ(type.memberOne.memberTwo, 200);
    EXPECT_EQ(type.memberTwo, 99);
}