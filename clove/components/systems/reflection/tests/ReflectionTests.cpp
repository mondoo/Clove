#include <Clove/Reflection/Reflection.hpp>
#include <gtest/gtest.h>

using namespace clove;

struct TestAttribute {};

class PublicReflectClass {
public:
    int x;
    float y;
};

CLOVE_REFLECT_BEGIN(PublicReflectClass, TestAttribute{})
CLOVE_REFLECT_PROPERTY(x, TestAttribute{})
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

    EXPECT_EQ(reflection::TypeInfo<PublicReflectClass>::memberCount, memberCount);
}

TEST(ReflectionTests, CanGetNumClassPrivateMembers) {
    size_t constexpr memberCount{ 2 };

    EXPECT_EQ(reflection::TypeInfo<PublicReflectClass>::memberCount, memberCount);
}

TEST(ReflectionTests, CanGetClassAttributes) {
    EXPECT_TRUE(reflection::hasAttribute<TestAttribute>(reflection::TypeInfo<PublicReflectClass>{}));
    EXPECT_FALSE(reflection::hasAttribute<TestAttribute>(reflection::TypeInfo<PrivateReflectClass>{}));
}

TEST(ReflectionTests, CanGetBasicMemberInfo) {
    auto publicMembers{ reflection::TypeInfo<PublicReflectClass>::getMembers() };

    EXPECT_EQ(std::get<0>(publicMembers).name, "x");
    EXPECT_EQ(std::get<0>(publicMembers).offset, offsetof(PublicReflectClass, x));
    EXPECT_EQ(std::get<0>(publicMembers).size, sizeof(PublicReflectClass::x));

    EXPECT_EQ(std::get<1>(publicMembers).name, "y");
    EXPECT_EQ(std::get<1>(publicMembers).offset, offsetof(PublicReflectClass, y));
    EXPECT_EQ(std::get<1>(publicMembers).size, sizeof(PublicReflectClass::y));

    auto privateMembers{ reflection::TypeInfo<PrivateReflectClass>::getMembers() };
    PrivateReflectClass sizeHelper{};

    EXPECT_EQ(std::get<0>(privateMembers).name, "a");
    EXPECT_EQ(std::get<0>(privateMembers).offset, sizeHelper.offsetOfA());
    EXPECT_EQ(std::get<0>(privateMembers).size, sizeHelper.sizeOfA());

    EXPECT_EQ(std::get<1>(privateMembers).name, "b");
    EXPECT_EQ(std::get<1>(privateMembers).offset, sizeHelper.offsetOfB());
    EXPECT_EQ(std::get<1>(privateMembers).size, sizeHelper.sizeOfB());
}

TEST(ReflectionTests, CanGetMemberAttributes) {
    auto members{ reflection::TypeInfo<PublicReflectClass>::getMembers() };

    EXPECT_TRUE(reflection::hasAttribute<TestAttribute>(std::get<0>(members)));
    EXPECT_FALSE(reflection::hasAttribute<TestAttribute>(std::get<1>(members)));
}

TEST(ReflectionTests, CanInterateOverClassMembers) {
    int32_t totalMemberCount{ 0 };
    int32_t attributeMemberCount{ 0 };

    reflection::forEachMember<PublicReflectClass>([&](auto &member) {
        ++totalMemberCount;
        if(reflection::hasAttribute<TestAttribute>(member)) {
            ++attributeMemberCount;
        }
    });

    EXPECT_EQ(totalMemberCount, 2);
    EXPECT_EQ(attributeMemberCount, 1);
}