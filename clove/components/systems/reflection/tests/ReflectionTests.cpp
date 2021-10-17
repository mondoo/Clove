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

    EXPECT_EQ(clove::MetaClass<PublicReflectClass>::memberCount, memberCount);
}

TEST(ReflectionTests, CanGetNumClassPrivateMembers) {
    size_t constexpr memberCount{ 2 };

    EXPECT_EQ(clove::MetaClass<PublicReflectClass>::memberCount, memberCount);
}

TEST(ReflectionTests, CanGetBasicPropertyInfo) {
    auto publicProps{ clove::MetaClass<PublicReflectClass>::getMembers() };

    EXPECT_EQ(std::get<0>(publicProps).name, "x");
    EXPECT_EQ(std::get<0>(publicProps).offset, offsetof(PublicReflectClass, x));
    EXPECT_EQ(std::get<0>(publicProps).size, sizeof(PublicReflectClass::x));

    EXPECT_EQ(std::get<1>(publicProps).name, "y");
    EXPECT_EQ(std::get<1>(publicProps).offset, offsetof(PublicReflectClass, y));
    EXPECT_EQ(std::get<1>(publicProps).size, sizeof(PublicReflectClass::y));

    auto privateProps{ clove::MetaClass<PrivateReflectClass>::getMembers() };
    PrivateReflectClass sizeHelper{};

    EXPECT_EQ(std::get<0>(privateProps).name, "a");
    EXPECT_EQ(std::get<0>(privateProps).offset, sizeHelper.offsetOfA());
    EXPECT_EQ(std::get<0>(privateProps).size, sizeHelper.sizeOfA());

    EXPECT_EQ(std::get<1>(privateProps).name, "b");
    EXPECT_EQ(std::get<1>(privateProps).offset, sizeHelper.offsetOfB());
    EXPECT_EQ(std::get<1>(privateProps).size, sizeHelper.sizeOfB());
}