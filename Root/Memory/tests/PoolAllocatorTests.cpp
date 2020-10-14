#include <Root/Memory/PoolAllocator.hpp>
#include <array>
#include <gtest/gtest.h>

using namespace garlic;

TEST(PoolAllocatorTests, CanAllocateAnItem) {
    const size_t numOfItems = 100;

    PoolAllocator<float> allocator(numOfItems);

    float* data = allocator.alloc();

    EXPECT_TRUE(data != nullptr);
}

TEST(PoolAllocatorTests, CanAllocateAnItemAndConstructItem) {
    constexpr int xVal   = 4;
    constexpr float bVal = 32.333f;
    struct Data {
        Data() = default;
        Data(int x, float b)
            : x(x)
            , b(b) {
        }

        int x{ 0 };
        float b{ 0.0f };
    };

    const size_t numOfItems = 100;
    PoolAllocator<Data> allocator(numOfItems);

    Data* dataA = allocator.alloc();
    Data* dataB = allocator.alloc(xVal, bVal);

    EXPECT_EQ(dataA->x, 0);
    EXPECT_EQ(dataA->b, 0.0f);

    EXPECT_NE(dataB->x, 0);
    EXPECT_NE(dataB->b, 0.0f);
    EXPECT_EQ(dataB->x, xVal);
    EXPECT_EQ(dataB->b, bVal);
}

TEST(PoolAllocatorTests, CallsAnItemsDestructor) {
    struct Helper {
        Helper(bool* flag)
            : flag(flag) {
        }
        ~Helper() {
            *flag = true;
        }

        bool* flag{ nullptr };
    };

    bool wasDeconstructorCalled{ false };

    const size_t numOfItems = 100;
    PoolAllocator<Helper> allocator(numOfItems);

    Helper* data = allocator.alloc(&wasDeconstructorCalled);

    EXPECT_TRUE(data->flag);

    allocator.free(data);

    EXPECT_TRUE(wasDeconstructorCalled);
}

TEST(PoolAllocatorTests, CannotAllocateMoreThanTheAllocatorHas) {
    const size_t numOfItems = 5;

    PoolAllocator<float> allocator(numOfItems);

    for(size_t i = 0; i < numOfItems; ++i) {
        allocator.alloc();
    }

    float* data = allocator.alloc();

    EXPECT_FALSE(data != nullptr);
}

TEST(PoolAllocatorTests, CanFreeAndReallocateItems) {
    const size_t numOfItems = 5;

    PoolAllocator<float> allocator(numOfItems);

    float* lastElem{ nullptr };
    for(size_t i = 0; i < numOfItems; ++i) {
        lastElem = allocator.alloc();
    }

    EXPECT_TRUE(lastElem != nullptr);

    allocator.free(lastElem);

    float* someData = allocator.alloc();

    EXPECT_TRUE(someData != nullptr);
}

TEST(PoolAllocatorTests, CanFreeAndReallocateAnyItem) {
    const size_t numOfItems = 10;
    const size_t allocIndex = 5;

    PoolAllocator<float> allocator(numOfItems);

    float* middleElem{ nullptr };
    for(size_t i = 0; i < numOfItems; ++i) {
        if(i == allocIndex) {
            middleElem = allocator.alloc();
        } else {
            auto* discarded = allocator.alloc();
        }
    }

    EXPECT_TRUE(middleElem != nullptr);

    allocator.free(middleElem);

    float* someData = allocator.alloc();

    EXPECT_TRUE(someData != nullptr);
}

TEST(PoolAllocatorTests, CanInitialiseWithARangeOfMemory) {
    std::array<std::byte, 100> allocatorRange;
    PoolAllocator<uint32_t> allocator(allocatorRange.data(), allocatorRange.size() / sizeof(uint32_t));

    const uint32_t dataValue = 100;

    uint32_t* data = allocator.alloc(dataValue);

    EXPECT_TRUE(data != nullptr);
    EXPECT_EQ(dataValue, *data);
}