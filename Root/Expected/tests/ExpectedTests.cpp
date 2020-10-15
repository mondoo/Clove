#include <Root/Expected.hpp>
#include <gtest/gtest.h>

using namespace garlic;

TEST(ExpectedTests, CanConstructWithAValue) {
    int32_t constexpr testValue = 50;
    Expected<int32_t, std::exception> expectedValue(testValue);

    EXPECT_EQ(testValue, expectedValue.getValue());
}

TEST(ExpectedTests, CanConstructWithAnError) {
    std::runtime_error error("Test Error");
    Expected<int32_t, std::runtime_error> expectedValue(Unexpected{error}));

    EXPECT_EQ(error, expectedValue.getError());
}