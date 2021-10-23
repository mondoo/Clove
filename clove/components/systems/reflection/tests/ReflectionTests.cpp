#include <Clove/Reflection/Reflection.hpp>
#include <gtest/gtest.h>

using namespace clove;

struct TestAttribute {
    std::string text{};
};

class PublicReflectClass {
public:
    int x;
    float y;
};

CLOVE_REFLECT_BEGIN(PublicReflectClass, TestAttribute{ .text = "class" })
CLOVE_REFLECT_PROPERTY(x, TestAttribute{ .text = "member" })
CLOVE_REFLECT_PROPERTY(y)
CLOVE_REFLECT_END(PublicReflectClass)

class PrivateReflectClass {
    CLOVE_REFLECT_PRIVATE(PrivateReflectClass)

private:
    int a;
    int b;

public:
    //Helper functions for tests
    size_t offsetOfA() const {
        return offsetof(PrivateReflectClass, a);
    };
    size_t sizeOfA() const {
        return sizeof(a);
    };

    size_t offsetOfB() const {
        return offsetof(PrivateReflectClass, b);
    };
    size_t sizeOfB() const {
        return sizeof(b);
    };
};

CLOVE_REFLECT_BEGIN(PrivateReflectClass)
CLOVE_REFLECT_PROPERTY(a)
CLOVE_REFLECT_PROPERTY(b)
CLOVE_REFLECT_END(PrivateReflectClass)

class NotReflectedClass {
public:
    int x;
    float y;
};

TEST(ReflectionTests, CanGetTypeInfoByType) {
    reflection::TypeInfo const publicClassInfo{ reflection::getTypeInfo<PublicReflectClass>() };
    reflection::TypeInfo const privateClassInfo{ reflection::getTypeInfo<PrivateReflectClass>() };
    //reflection::TypeInfo const notReflectedClassInfo{ reflection::getTypeInfo<NotReflectedClass>() };

    //Test cannot compile if used on not reflected types
    EXPECT_TRUE(true);
}

TEST(ReflectionTests, CanGetTypeInfoByName){
    std::optional<reflection::TypeInfo> const publicClassInfo{ reflection::getTypeInfo("PublicReflectClass") };
    std::optional<reflection::TypeInfo> const privateClassInfo{ reflection::getTypeInfo("PrivateReflectClass") };
    std::optional<reflection::TypeInfo> const notReflectedClassInfo{ reflection::getTypeInfo("NotReflectedClass") };

    EXPECT_TRUE(publicClassInfo.has_value());
    EXPECT_TRUE(privateClassInfo.has_value());
    EXPECT_FALSE(notReflectedClassInfo.has_value());
}

TEST(ReflectionTests, CanGetTypeAttributes) {
    //TODO
}

TEST(ReflectionTests, CanGetValueOfTypeAttributes) {
    //TODO
}

TEST(ReflectionTests, CanGetNumTypePublicMembers) {
    size_t constexpr memberCount{ 2 };
    reflection::TypeInfo const publicClassInfo{ reflection::getTypeInfo<PublicReflectClass>() };

    EXPECT_EQ(publicClassInfo.members.size(), memberCount);
}

TEST(ReflectionTests, CanGetNumTypePrivateMembers) {
    size_t constexpr memberCount{ 2 };
    reflection::TypeInfo const privateClassInfo{ reflection::getTypeInfo<PrivateReflectClass>() };

    EXPECT_EQ(privateClassInfo.members.size(), memberCount);
}

TEST(ReflectionTests, CanGetBasicMemberInfo) {
    std::vector<reflection::MemberInfo> const &publicClassMembers{ reflection::getTypeInfo<PublicReflectClass>().members };

    EXPECT_EQ(publicClassMembers[0].name, "x");
    EXPECT_EQ(publicClassMembers[0].offset, offsetof(PublicReflectClass, x));
    EXPECT_EQ(publicClassMembers[0].size, sizeof(PublicReflectClass::x));

    EXPECT_EQ(publicClassMembers[1].name, "y");
    EXPECT_EQ(publicClassMembers[1].offset, offsetof(PublicReflectClass, y));
    EXPECT_EQ(publicClassMembers[1].size, sizeof(PublicReflectClass::y));

    std::vector<reflection::MemberInfo> const &privateClassMembers{ reflection::getTypeInfo<PrivateReflectClass>().members };
    PrivateReflectClass sizeHelper{};

    EXPECT_EQ(privateClassMembers[0].name, "a");
    EXPECT_EQ(privateClassMembers[0].offset, sizeHelper.offsetOfA());
    EXPECT_EQ(privateClassMembers[0].size, sizeHelper.sizeOfA());

    EXPECT_EQ(privateClassMembers[1].name, "b");
    EXPECT_EQ(privateClassMembers[1].offset, sizeHelper.offsetOfB());
    EXPECT_EQ(privateClassMembers[1].size, sizeHelper.sizeOfB());
}

TEST(ReflectionTests, CanGetMemberAttributes) {
    //TODO
}

TEST(ReflectionTests, CanGetValueOfMemberAttributes) {
    //TODO
}