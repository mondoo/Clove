#pragma once

namespace blb::ai {
	using BlackBoardKey = size_t;

	struct BlackBoardDataHeader {
		void* prevItem = nullptr;
		void* nextItem = nullptr;

		size_t key = 0;
		size_t size = 0;
	};

	class BlackBoard {
		//VARIABLES
	private:
		//Idea is that we store data in here with a header value so we know the memory layout
		void* data = nullptr;
		void* pointer = nullptr; //Points to current free space

		//FUNCTIONS
	public:
		//TODO: Ctors
		BlackBoard() {
			data = malloc(1024 * 1024); //Lets just give ourselves a meg at the moment
		}

		template<typename DataType>
		BlackBoardKey addItem(DataType item) {
		}

		template<typename DataType>
		DataType& getItem(BlackBoardKey key) {
		}

		void removeItem(BlackBoardKey key) {
		}

	private:
	};
}