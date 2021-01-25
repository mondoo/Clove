#include <Clove/Reflection/Reflect.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

struct UnkownTestClass{
    int32_t member;
};

TEST(ReflectionTests, CantGetInformationOnAnUnkownType) {
    UnkownTestClass testClass{};
    Type typeInfo{ reflect(testClass) };

    EXPECT_EQ(typeInfo.name, "");
    EXPECT_NE(typeInfo.size, sizeof(UnkownTestClass));
    EXPECT_EQ(typeInfo.fields.size(), 0);
}

struct SimpleTestClass {
    int32_t intMember;
};
CLOVE_REFLECT_TYPE(SimpleTestClass)
CLOVE_REFLECT_END

TEST(ReflectionTests, CanGetBasicObjectInformationOnAType) {
    SimpleTestClass testClass{};
    Type typeInfo{ reflect(testClass) };

    EXPECT_EQ(typeInfo.name, "SimpleTestClass");
    EXPECT_EQ(typeInfo.size, sizeof(SimpleTestClass));
    EXPECT_EQ(typeInfo.fields.size(), 0);
}

struct SimpleMemberTestClass {
    int32_t intMember;
    float floatMember;
};
CLOVE_REFLECT_TYPE(SimpleMemberTestClass)
CLOVE_REFLECT_FIELD(intMember)
CLOVE_REFLECT_FIELD(floatMember)
CLOVE_REFLECT_END

TEST(ReflectionTests, CanGetBasicObjectMemberInformationOnAType) {
    SimpleMemberTestClass testClass{
        .intMember   = 42,
        .floatMember = 32.5f,
    };
    Type typeInfo{ reflect(testClass) };

    ASSERT_EQ(typeInfo.fields.size(), 2);

    EXPECT_EQ(typeInfo.fields[0].name, "intMember");
    EXPECT_EQ(typeInfo.fields[0].size, sizeof(SimpleMemberTestClass::intMember));
    EXPECT_EQ(typeInfo.fields[0].offset, offsetof(SimpleMemberTestClass, intMember));
    EXPECT_EQ(std::any_cast<int32_t>(typeInfo.fields[0].value), testClass.intMember);

    EXPECT_EQ(typeInfo.fields[1].name, "floatMember");
    EXPECT_EQ(typeInfo.fields[1].size, sizeof(SimpleMemberTestClass::floatMember));
    EXPECT_EQ(typeInfo.fields[1].offset, offsetof(SimpleMemberTestClass, floatMember));
    EXPECT_EQ(std::any_cast<float>(typeInfo.fields[1].value), testClass.floatMember);
}