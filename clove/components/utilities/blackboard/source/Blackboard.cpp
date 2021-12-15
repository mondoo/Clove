#include "Clove/Blackboard.hpp"

namespace {
    size_t constexpr defaultMemorySize{ 1024 };
}

namespace clove {
    Blackboard::Blackboard()
        : Blackboard(defaultMemorySize) {
    }

    Blackboard::Blackboard(size_t memorySize)
        : memoryBlock(memorySize) {
    }

    Blackboard::Blackboard(Blackboard &&other) noexcept
        : memoryBlock(std::move(other.memoryBlock)) {
        dataMap = std::move(other.dataMap);
    }

    Blackboard &Blackboard::operator=(Blackboard &&other) noexcept {
        memoryBlock = std::move(other.memoryBlock);
        dataMap     = std::move(other.dataMap);

        return *this;
    }

    Blackboard::~Blackboard() = default;

    bool Blackboard::hasValue(Key key) const {
        return dataMap.find(key) != dataMap.end();
    }
}