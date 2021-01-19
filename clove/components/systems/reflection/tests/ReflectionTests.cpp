#include <Clove/Reflection/Reflect.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

struct NonReflectedClass {
    float member;
};

TEST(ReflectionTests, GetClassWithoutReflecting) {
    NonReflectedClass testClass{};
    Class testClassInfo{ getClass(testClass) };

    EXPECT_EQ(testClassInfo.name, "");
    EXPECT_EQ(testClassInfo.size, 0);
    EXPECT_EQ(testClassInfo.members.size(), 0);

    int32_t basicType{};
    Class basicTypeClass{ getClass(basicType) };

    EXPECT_EQ(basicTypeClass.name, "");
    EXPECT_EQ(basicTypeClass.size, 0);
    EXPECT_EQ(basicTypeClass.members.size(), 0);
}

struct SimpleTestClass {
    int32_t intMember;
};
CLOVE_REFLECT_CLASS(SimpleTestClass);

TEST(ReflectionTests, CanGetBasicInformationOnAType) {
    SimpleTestClass testClass{};
    Class testClassInfo{ getClass(testClass) };

    EXPECT_EQ(testClassInfo.name, "SimpleTestClass");
    EXPECT_EQ(testClassInfo.size, sizeof(SimpleTestClass));
    EXPECT_EQ(testClassInfo.members.size(), 0);
}

struct SimpleMemberTestClass {
    int32_t intMember;
    float floatMember;
};
CLOVE_REFLECT_CLASS(
    SimpleMemberTestClass,
    CLOVE_REFLECT_CLASS_MEMBER(SimpleMemberTestClass, intMember),
    CLOVE_REFLECT_CLASS_MEMBER(SimpleMemberTestClass, floatMember), 
);

TEST(ReflectionTests, CanGetBasicMemberInformationOnAType) {
    SimpleMemberTestClass testClass{};
    Class testClassInfo{ getClass(testClass) };

    ASSERT_EQ(testClassInfo.members.size(), 2);
    EXPECT_EQ(testClassInfo.members[0].name, "intMember");
    EXPECT_EQ(testClassInfo.members[0].size, sizeof(SimpleMemberTestClass::intMember));
    EXPECT_EQ(testClassInfo.members[0].offset, offsetof(SimpleMemberTestClass, intMember));
    EXPECT_EQ(testClassInfo.members[1].name, "floatMember");
    EXPECT_EQ(testClassInfo.members[1].size, sizeof(SimpleMemberTestClass::floatMember));
    EXPECT_EQ(testClassInfo.members[1].offset, offsetof(SimpleMemberTestClass, floatMember));
}