#include <Clove/Reflection/Reflect.hpp>
#include <gtest/gtest.h>

using namespace garlic::clove;

struct SimpleTestClass {
    int32_t intMember;
};
CLOVE_REFLECT_CLASS(SimpleTestClass);

TEST(ReflectionTests, CanGetBasicInformationOnAType) {
    Class testClassInfo{ getClass<SimpleTestClass>() };
    EXPECT_EQ(testClassInfo.name, "SimpleTestClass");
    EXPECT_EQ(testClassInfo.size, sizeof(SimpleTestClass));
}

//Get member names?
//Get user members
//Get nested members (declared inside classes?)
//Get pointer members

//Get member functions

//Inheritance?
//Templated types/functions?