#include "Clove/BlackBoard.hpp"

namespace {
    size_t constexpr defaultMemorySize{ 1024 };
}

namespace clove {
    BlackBoard::BlackBoard()
        : BlackBoard(defaultMemorySize) {
    }

    BlackBoard::BlackBoard(size_t memorySize)
        : memoryBlock(memorySize) {
    }

    BlackBoard::BlackBoard(BlackBoard &&other) noexcept
        : memoryBlock(std::move(other.memoryBlock)) {
        dataMap = std::move(other.dataMap);
    }

    BlackBoard &BlackBoard::operator=(BlackBoard &&other) noexcept {
        memoryBlock = std::move(other.memoryBlock);
        dataMap     = std::move(other.dataMap);

        return *this;
    }

    BlackBoard::~BlackBoard() = default;

    bool BlackBoard::hasValue(Key key) const {
        return dataMap.find(key) != dataMap.end();
    }
}