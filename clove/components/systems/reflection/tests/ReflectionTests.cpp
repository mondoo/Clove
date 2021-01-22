#include <Clove/Reflection/Reflect.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

struct SimpleTestClass {
    int32_t intMember;
};
CLOVE_REFLECT_TYPE(SimpleTestClass);
CLOVE_REFLECT_END

TEST(ReflectionTests, CanGetBasicStaticInformationOnAType) {
    EXPECT_EQ(typeid(TypeInfo<SimpleTestClass>::Type).name(), typeid(SimpleTestClass).name());
    EXPECT_EQ(TypeInfo<SimpleTestClass>::name, "SimpleTestClass");
    EXPECT_EQ(TypeInfo<SimpleTestClass>::size, sizeof(SimpleTestClass));
    EXPECT_EQ(TypeInfo<SimpleTestClass>::memberCount, 0);
}

TEST(ReflectionTests, CanGetBasicObjectInformationOnAType) {
    SimpleTestClass testClass{};
    TypeInfo<SimpleTestClass> typeInfo{ getTypeInfo(testClass) };
    auto members{ typeInfo.getMembers() };

    EXPECT_EQ(typeInfo.name, "SimpleTestClass");
    EXPECT_EQ(typeInfo.size, sizeof(SimpleTestClass));
    EXPECT_EQ(typeInfo.memberCount, 0);
    EXPECT_EQ(std::tuple_size_v<decltype(members)>, typeInfo.memberCount);
}

struct SimpleMemberTestClass {
    int32_t intMember;
    float floatMember;
};
CLOVE_REFLECT_TYPE(SimpleMemberTestClass)
CLOVE_REFLECT_MEMBER(intMember)
CLOVE_REFLECT_MEMBER(floatMember)
CLOVE_REFLECT_END

TEST(ReflectionTests, CanGetBasicStaticMemberInformationOnAType) {
    ASSERT_EQ(TypeInfo<SimpleMemberTestClass>::memberCount, 2);

    EXPECT_EQ(typeid(TypeInfo<SimpleMemberTestClass>::MemberInfo<0>::Type).name(), typeid(SimpleMemberTestClass::intMember).name());
    EXPECT_EQ(TypeInfo<SimpleMemberTestClass>::MemberInfo<0>::name, "intMember");
    EXPECT_EQ(TypeInfo<SimpleMemberTestClass>::MemberInfo<0>::size, sizeof(SimpleMemberTestClass::intMember));
    EXPECT_EQ(TypeInfo<SimpleMemberTestClass>::MemberInfo<0>::offset, offsetof(SimpleMemberTestClass, intMember));

    EXPECT_EQ(typeid(TypeInfo<SimpleMemberTestClass>::MemberInfo<1>::Type).name(), typeid(SimpleMemberTestClass::floatMember).name());
    EXPECT_EQ(TypeInfo<SimpleMemberTestClass>::MemberInfo<1>::name, "floatMember");
    EXPECT_EQ(TypeInfo<SimpleMemberTestClass>::MemberInfo<1>::size, sizeof(SimpleMemberTestClass::floatMember));
    EXPECT_EQ(TypeInfo<SimpleMemberTestClass>::MemberInfo<1>::offset, offsetof(SimpleMemberTestClass, floatMember));
}

TEST(ReflectionTests, CanGetBasicObjectMemberInformationOnAType) {
    SimpleMemberTestClass testClass{
        .intMember   = 42,
        .floatMember = 32.5f,
    };
    TypeInfo<SimpleMemberTestClass> typeInfo{ getTypeInfo(testClass) };

    auto members{ typeInfo.getMembers() };

    ASSERT_EQ(typeInfo.memberCount, 2);
    ASSERT_EQ(std::tuple_size_v<decltype(members)>, typeInfo.memberCount);

    EXPECT_EQ(std::get<0>(members).name, "intMember");
    EXPECT_EQ(std::get<0>(members).size, sizeof(SimpleMemberTestClass::intMember));
    EXPECT_EQ(std::get<0>(members).offset, offsetof(SimpleMemberTestClass, intMember));
    //EXPECT_EQ(std::get<0>(members).value, testClass.intMember);

    EXPECT_EQ(std::get<1>(members).name, "floatMember");
    EXPECT_EQ(std::get<1>(members).size, sizeof(SimpleMemberTestClass::floatMember));
    EXPECT_EQ(std::get<1>(members).offset, offsetof(SimpleMemberTestClass, floatMember));
    //EXPECT_EQ(std::get<1>(members).value, testClass.floatMember);
}

//TODO: Loop through members