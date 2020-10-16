#include <Root/Expected.hpp>
#include <gtest/gtest.h>

using namespace garlic;

TEST(ExpectedTests, CanConstructWithAValue) {
    int32_t constexpr testValue = 50;
    Expected<int32_t, std::exception> expectedValue(testValue);

    EXPECT_EQ(testValue, expectedValue.getValue());
}

TEST(ExpectedTests, CanConstructWithAnError) {
    using ErrorType           = int32_t;
    ErrorType constexpr error = 42;

    Expected<int32_t, ErrorType> expectedValue(Unexpected<ErrorType>{ error });

    EXPECT_EQ(error, expectedValue.getError());
}