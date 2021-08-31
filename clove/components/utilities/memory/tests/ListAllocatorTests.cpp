#include <Clove/Memory/ListAllocator.hpp>
#include <array>
#include <gtest/gtest.h>

using namespace clove;

TEST(ListAllocatorTests, CanAllocateAnAmountOfBytes) {
    size_t constexpr allocatorSize{ 256 * 1024 };//256kb
    ListAllocator allocator(allocatorSize);

    void *data1{ allocator.alloc(100, 0) };
    uint32_t *data2{ allocator.alloc<uint32_t>() };

    EXPECT_TRUE(data1 != nullptr);
    EXPECT_TRUE(data2 != nullptr);
}

TEST(ListAllocatorTests, AllocationsAreProperlyAligned) {
    size_t constexpr allocatorSize{ 256 * 1024 };//256kb
    ListAllocator allocator(allocatorSize);

    size_t constexpr alignment{ 16 };

    void *data1{ allocator.alloc(100, alignment) };
    uint32_t *data2{ allocator.alloc<uint32_t>() };

    EXPECT_EQ(reinterpret_cast<uintptr_t>(data1) % alignment, 0);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(data2) % alignof(uint32_t), 0);
}

TEST(ListAllocatorTests, MultipleAllocationsDoNotOverlap) {
    size_t constexpr allocatorSize{ 256 * 1024 };//256kb
    ListAllocator allocator(allocatorSize);

    size_t constexpr alignment{ 16 };

    auto *data1{ allocator.alloc<uint32_t>() };
    auto *data2{ allocator.alloc<uint64_t>() };
    auto *data3{ allocator.alloc<float>() };
    auto *data4{ allocator.alloc<uint8_t>() };
    auto *data5{ allocator.alloc<uint32_t>() };
    auto *data6{ allocator.alloc<uint32_t>() };

    uint32_t constexpr data1Value{ 100000000 };
    uint64_t constexpr data2Value{ 12345678987654321 };
    float constexpr data3Value{ 3.13754943 };
    uint8_t constexpr data4Value{ 19 };
    uint32_t constexpr data5Value{ 10001 };
    uint32_t constexpr data6Value{ 1002 };

    *data1 = data1Value;
    *data2 = data2Value;
    *data3 = data3Value;
    *data4 = data4Value;
    *data5 = data5Value;
    *data6 = data6Value;

    EXPECT_EQ(*data1, data1Value);
    EXPECT_EQ(*data2, data2Value);
    EXPECT_EQ(*data3, data3Value);
    EXPECT_EQ(*data4, data4Value);
    EXPECT_EQ(*data5, data5Value);
    EXPECT_EQ(*data6, data6Value);
}

TEST(ListAllocatorTests, CannotAllocateMoreThanTheAllocatorHas) {
    size_t constexpr allocatorSize{ 256 * 1024 };//256kb
    ListAllocator allocator(allocatorSize);

    void *data{ allocator.alloc(allocatorSize + 1, 0) };

    EXPECT_FALSE(data != nullptr);
}

TEST(ListAllocatorTests, CanFreeAndReallocateBytes) {
    size_t constexpr allocatorSize{ 256 * 1024 };//256kb
    ListAllocator allocator(allocatorSize);

    void *allData{ allocator.alloc(allocatorSize, 0) };

    EXPECT_TRUE(allData != nullptr);

    allocator.free(allData);

    void *someData{ allocator.alloc(500, 0) };

    EXPECT_TRUE(someData != nullptr);
}

TEST(ListAllocatorTests, CanInitialiseWithARangeOfMemory) {
    std::array<std::byte, 256 * 1024> allocatorRange;
    ListAllocator allocator(allocatorRange.data(), allocatorRange.size());

    void *data{ allocator.alloc(100, 0) };

    EXPECT_TRUE(data != nullptr);
}

TEST(ListAllocatorTests, FreeWillFreeAndReallocTheCorrectRange) {
    size_t constexpr allocatorSize{ 256 * 1024 };//256kb
    ListAllocator allocator(allocatorSize);

    uint32_t constexpr value{ 5 };

    uint32_t *first{ allocator.alloc<uint32_t>() };
    *first = value;

    allocator.free(first);

    uint32_t *second{ allocator.alloc<uint32_t>() };

    EXPECT_EQ(value, *second);
}