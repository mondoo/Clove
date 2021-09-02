#pragma once

#include <cstddef>

namespace clove {
    /**
     * @brief A memory allocator that is good for temporary allocations.
     * @details A StackAllocator allows any sized allocation but cannot free specific blocks of memory.
     * Instead you must either free the entire allocator or free everything beyond a certain marker.
     */
    class StackAllocator {
        //TYPES
    public:
        using Marker = size_t;

        //VARIABLES
    private:
        std::byte *stack{};
        size_t stackSize{ 0 };
        std::byte *top{ nullptr };

        bool freeMemory{ true };

        //FUNCTIONS
    public:
        StackAllocator() = delete;
        StackAllocator(size_t sizeBytes);
        StackAllocator(std::byte *start, size_t sizeBytes);

        StackAllocator(StackAllocator const &other) = delete;
        StackAllocator(StackAllocator &&other) noexcept;

        StackAllocator &operator=(StackAllocator const &other) = delete;
        StackAllocator &operator=(StackAllocator &&other) noexcept;

        ~StackAllocator();

        /**
         * @brief Creates a marker of the current stack position of the allocator.
         * @returns 
         */
        Marker markPosition();

        /**
         * @brief Allocates a chunk of memory of the required size and alignment.
         * @param size 
         * @param alignment
         * @return Pointer to the begining of the chunk of memory
         */
        void *alloc(size_t size, size_t alignment);

        /**
         * @brief Allocates enough memory for a type T.
         * @tparam T 
         * @return 
         */
        template<typename T>
        T *alloc();

        /**
         * @brief Free the entire allocator, effectively resetting the stack.
         */
        void free();
        /**
         * @brief Free everything allocated after the marker.
         * @param marker
         */
        void free(Marker marker);
    };
}

#include "StackAllocator.inl"