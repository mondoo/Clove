#include <Clove/Reflection/Reflection.hpp>
#include <gtest/gtest.h>

using namespace clove;

class PublicReflectClass {
public:
    int x;
    float y;
};

CLOVE_REFLECT_BEGIN(PublicReflectClass)
    CLOVE_REFLECT_PROPERTY(x)
    CLOVE_REFLECT_PROPERTY(y)
CLOVE_REFLECT_END

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
CLOVE_REFLECT_END

TEST(ReflectionTests, CanGetNumClassPublicMembers) {
    size_t constexpr memberCount{ 2 };

    clove::MetaClass<PublicReflectClass> classInfo{};

    EXPECT_EQ(classInfo.getProperties().size(), memberCount);
}

TEST(ReflectionTests, CanGetNumClassPrivateMembers) {
    size_t constexpr memberCount{ 2 };

    clove::MetaClass<PrivateReflectClass> classInfo{};

    EXPECT_EQ(classInfo.getProperties().size(), memberCount);
}

TEST(ReflectionTests, CanGetBasicPropertyInfo) {
    clove::MetaClass<PublicReflectClass> publicClassInfo{};
    auto &publicProps{ publicClassInfo.getProperties() };

    EXPECT_EQ(publicProps[0].name, "x");
    EXPECT_EQ(publicProps[0].offset, offsetof(PublicReflectClass, x));
    EXPECT_EQ(publicProps[0].size, sizeof(PublicReflectClass::x));

    EXPECT_EQ(publicProps[1].name, "y");
    EXPECT_EQ(publicProps[1].offset, offsetof(PublicReflectClass, y));
    EXPECT_EQ(publicProps[1].size, sizeof(PublicReflectClass::y));

    clove::MetaClass<PrivateReflectClass> privateClassInfo{};
    auto &privateProps{ privateClassInfo.getProperties() };
    PrivateReflectClass sizeHelper{};

    EXPECT_EQ(privateProps[0].name, "a");
    EXPECT_EQ(privateProps[0].offset, sizeHelper.offsetOfA());
    EXPECT_EQ(privateProps[0].size, sizeHelper.sizeOfA());

    EXPECT_EQ(privateProps[1].name, "b");
    EXPECT_EQ(privateProps[1].offset, sizeHelper.offsetOfB());
    EXPECT_EQ(privateProps[1].size, sizeHelper.sizeOfB());
}