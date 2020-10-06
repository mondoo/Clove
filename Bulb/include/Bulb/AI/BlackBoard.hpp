#pragma once

#include <Root/Memory/ListAllocator.hpp>

namespace blb::ai {
    class BlackBoard {
        //TYPES
    public:
        using Key = size_t;

        //VARIABLES
    private:
        garlic::ListAllocator memoryBlock;

        std::map<Key, void*> dataMap;

        //FUNCTIONS
    public:
        BlackBoard();
        BlackBoard(size_t memorySize);

        BlackBoard(const BlackBoard& other) = delete;
        BlackBoard(BlackBoard&& other) noexcept;

        BlackBoard& operator=(const BlackBoard& other) = delete;
        BlackBoard& operator=(BlackBoard&& other) noexcept;

        ~BlackBoard();

        template<typename DataType>
        void setValue(Key key, DataType value);
        template<typename DataType>
        std::optional<DataType> getValue(Key key);
    };
}

#include "BlackBoard.inl"