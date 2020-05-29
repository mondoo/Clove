#pragma once

namespace blb::ai {
	using BlackBoardKey = size_t;

	struct BlackBoardDataHeader {
		char* prevItem = nullptr;
		char* nextItem = nullptr;

		BlackBoardKey key = 0;
		size_t itemSize = 0;
	};

	class BlackBoard {
		//VARIABLES
	private:
		//Idea is that we store data in here with a header value so we know the memory layout
		char* data = nullptr;
		size_t dataSize = 0;
		char* pointer = nullptr; //Points to current free space

		size_t nextKey = 1;

		//FUNCTIONS
	public:
		//TODO: Ctors
		BlackBoard() {
			dataSize = 1024 * 1024; //Lets just give ourselves a meg at the moment
			data = reinterpret_cast<char*>(malloc(dataSize)); 
			pointer = data;
		}

		~BlackBoard(){
			free(data);
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
			header.itemSize = itemSize;

			//TODO: Doing memcpy for simplicity, try placement new
			memcpy(pointer, &header, headerSize);
			pointer += headerSize;
			memcpy(pointer, &item, itemSize);
			pointer += itemSize;

			return header.key;
		}

		template<typename DataType>
		DataType& getItem(BlackBoardKey key) {
			char* iter = data;
			while(iter < pointer) {
				auto* header = reinterpret_cast<BlackBoardDataHeader*>(iter);
				if(header->key == key) {
					auto* item = reinterpret_cast<DataType*>(iter + sizeof(BlackBoardDataHeader));
					return *item;
				} else {
					iter = header->nextItem;
				}
			}
		}

		void removeItem(BlackBoardKey key) {
		}

	private:
	};
}