#include <Clove/Reflection/Reflection.hpp>
#include <gtest/gtest.h>

using namespace clove;

class ReflectClass{
    public:
        int x;
        float y;
};

CLOVE_REFLECT_BEGIN(ReflectClass)
    CLOVE_REFLECT_PROPERTY(x)
    CLOVE_REFLECT_PROPERTY(y)
CLOVE_REFLECT_END

TEST(ReflectionTests, CanGetNumClassPublicMembers) {
    size_t constexpr memberCount{ 2 };
    
    clove::MetaClass<ReflectClass> classInfo{};

    EXPECT_EQ(classInfo.getProperties().size(), memberCount);
}