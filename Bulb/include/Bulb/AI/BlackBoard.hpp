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
		size_t dataSize = 0;
		void* pointer = nullptr; //Points to current free space

		size_t nextKey = 1;

		//FUNCTIONS
	public:
		//TODO: Ctors
		BlackBoard() {
			dataSize = 1024 * 1024; //Lets just give ourselves a meg at the moment
			data = malloc(dataSize); 
			pointer = data;
		}

		template<typename DataType>
		BlackBoardKey addItem(DataType item) {
			const size_t headerSize = sizeof(BlackBoardDataHeader);
			const size_t itemSize = sizeof(DataType);
			const size_t totalSize = headerSize + itemSize;
			CLV_ASSERT(pointer + totalSize > data + dataSize, "Not enough space!"); //TODO: Remove assertion?

			BlackBoardDataHeader header{};
			if(pointer != data) {
				header.prevItem = pointer;
				//TODO: Update nextItem of previous
			}
			header.key = nextKey++;
			header.size = itemSize;

			//TODO: Doing memcpy for simplicity, try placement new
			memcpy(pointer, &header, headerSize);
			pointer += headerSize;
			memcpy(pointer, &item, itemSize);
			pointer += itemSize;

			return header.key;
		}

		template<typename DataType>
		DataType& getItem(BlackBoardKey key) {
		}

		void removeItem(BlackBoardKey key) {
		}

	private:
	};
}