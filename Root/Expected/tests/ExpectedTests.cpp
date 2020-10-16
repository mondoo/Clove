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

TEST(ExpectedTests, CallsDestructorOnValueType) {
    struct Helper {
    public:
        bool &flag;

    public:
        Helper(bool &flag)
            : flag(flag) {
        }
        ~Helper() {
            flag = true;
        }
    };

    bool destructorCalled{ false };

    {
        Expected<Helper, std::exception> expected(Helper{ destructorCalled });
    }

    EXPECT_TRUE(destructorCalled);
}

TEST(ExpectedTests, CallsDestructorOnErrorType) {
    struct Helper {
    public:
        bool &flag;

    public:
        Helper(bool &flag)
            : flag(flag) {
        }
        ~Helper() {
            flag = true;
        }
    };

    bool destructorCalled{ false };

    {
        Expected<int32_t, Helper> expected(Unexpected<Helper>{ Helper{ destructorCalled } });
    }

    EXPECT_TRUE(destructorCalled);
}