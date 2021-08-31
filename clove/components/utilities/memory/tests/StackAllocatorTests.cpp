#include <Clove/Memory/StackAllocator.hpp>
#include <array>
#include <gtest/gtest.h>

using namespace clove;

static size_t constexpr allocatorSize{ 1024 };

TEST(StackAllocatorTests, CanAllocateAnAmountOfBytes) {
    StackAllocator allocator{ allocatorSize };

    void *data = allocator.alloc(100, 0);

    EXPECT_TRUE(data != nullptr);
}

TEST(StackAllocatorTests, AllocationsAreProperlyAligned) {
    StackAllocator allocator{ allocatorSize };

    size_t constexpr alignment{ 16 };

    void *data1{ allocator.alloc(100, alignment) };
    uint32_t *data2{ allocator.alloc<uint32_t>() };

    EXPECT_EQ(reinterpret_cast<uintptr_t>(data1) % alignment, 0);
    EXPECT_EQ(reinterpret_cast<uintptr_t>(data2) % alignof(uint32_t), 0);
}

TEST(StackAllocatorTests, CannotAllocateMoreThanTheAllocatorHas) {
    StackAllocator allocator{ allocatorSize };

    void *data{ allocator.alloc(allocatorSize + 1, 0) };

    EXPECT_FALSE(data != nullptr);
}

TEST(StackAllocatorTests, CanFreeAndReallocateBytes) {
    StackAllocator allocator{ allocatorSize };

    void *allData{ allocator.alloc(allocatorSize, 0) };

    EXPECT_TRUE(allData != nullptr);

    allocator.free();

    void *someData{ allocator.alloc(500, 0) };

    EXPECT_TRUE(someData != nullptr);
}

TEST(StackAllocatorTests, CanInitialiseWithARangeOfMemory) {
    std::array<std::byte, allocatorSize> allocatorRange;
    StackAllocator allocator{ allocatorRange.data(), allocatorRange.size() };

    void *data{ allocator.alloc(100, 0) };

    EXPECT_TRUE(data != nullptr);
}

TEST(StackAllocatorTests, CanFreeToAMarker) {
    StackAllocator allocator{ allocatorSize };

    float *a{ allocator.alloc<float>() };

    StackAllocator::Marker marker{ allocator.markPosition() };

    float *b{ allocator.alloc<float>() };

    *a = 1;
    *b = 2;

    allocator.free(marker);

    float *c{ allocator.alloc<float>() };

    *c = 3;

    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(*a, *b);
    EXPECT_NE(*a, *c);

    EXPECT_EQ(b, c) << "b and c should be pointing to the same adress";
    EXPECT_EQ(*b, *c) << "b and c should contain the same value";
}