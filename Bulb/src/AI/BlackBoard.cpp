#include "Bulb/AI/BlackBoard.hpp"

namespace blb::ai {
	BlackBoard::BlackBoard()
		: BlackBoard(1024) {
	}

	BlackBoard::BlackBoard(size_t memorySize)
		: memoryBlock(memorySize) {
	}

	BlackBoard::BlackBoard(BlackBoard&& other) noexcept = default;

	BlackBoard& BlackBoard::operator=(BlackBoard&& other) noexcept = default;

	BlackBoard::~BlackBoard() = default;

	void BlackBoard::removeItem(BlackBoard::Key key) {
		if(dataMap.find(key) == dataMap.end()) {
			return;
		}

		memoryBlock.free(dataMap[key]);
		dataMap.erase(key);
	}
}