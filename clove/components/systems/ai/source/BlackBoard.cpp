#include "Clove/AI/BlackBoard.hpp"

namespace garlic::clove {
    BlackBoard::BlackBoard()
        : BlackBoard(1024) {
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
}