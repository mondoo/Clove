#include <Clove/Reflection/Reflect.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

struct SimpleTestClass {
    int32_t intMember;
};
CLOVE_REFLECT_TYPE(SimpleTestClass);
CLOVE_REFLECT_END

TEST(ReflectionTests, CanGetStaticBasicInformationOnAType) {
    EXPECT_EQ(TypeInfo<SimpleTestClass>::name, "SimpleTestClass");
    EXPECT_EQ(TypeInfo<SimpleTestClass>::size, sizeof(SimpleTestClass));
    EXPECT_EQ(TypeInfo<SimpleTestClass>::memberCount, 0);
}

struct SimpleMemberTestClass {
    int32_t intMember;
    float floatMember;
};
CLOVE_REFLECT_TYPE(SimpleMemberTestClass)
CLOVE_REFLECT_MEMBER(intMember)
CLOVE_REFLECT_MEMBER(floatMember)
CLOVE_REFLECT_END

TEST(ReflectionTests, CanGetStaticBasicMemberInformationOnAType) {
    ASSERT_EQ(TypeInfo<SimpleMemberTestClass>::memberCount, 2);
    EXPECT_EQ(TypeInfo<SimpleMemberTestClass>::MemberInfo<0>::name, "intMember");
    //EXPECT_EQ(testClassInfo.members[0].size, sizeof(SimpleMemberTestClass::intMember));
    //EXPECT_EQ(testClassInfo.members[0].offset, offsetof(SimpleMemberTestClass, intMember));
    EXPECT_EQ(TypeInfo<SimpleMemberTestClass>::MemberInfo<1>::name, "floatMember");
    //EXPECT_EQ(testClassInfo.members[1].size, sizeof(SimpleMemberTestClass::floatMember));
    //EXPECT_EQ(testClassInfo.members[1].offset, offsetof(SimpleMemberTestClass, floatMember));
}

//TODO: Loop through members
//TODO: Get member values