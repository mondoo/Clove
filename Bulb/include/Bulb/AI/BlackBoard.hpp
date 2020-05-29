#pragma once

#include <Clove/Memory/ListAllocator.hpp>

namespace blb::ai {
	using BlackBoardKey = size_t;
	static constexpr BlackBoardKey INVALID_KEY = 0;

	class BlackBoard {
		//VARIABLES
	private:
		clv::mem::ListAllocator memoryBlock;
		BlackBoardKey nextKey = 1;

		std::map<BlackBoardKey, void*> dataMap;

		//FUNCTIONS
	public:
		//TODO: Ctors
		BlackBoard() 
			: memoryBlock(1024 * 1024) { //Lets just give ourselves a meg at the moment
		}

		~BlackBoard() = default;

		template<typename DataType>
		BlackBoardKey addItem(DataType item) {
			void* block = memoryBlock.alloc(sizeof(item));
			if(block == nullptr) {
				//TODO: Error
			}

			*reinterpret_cast<DataType*>(block) = item;

			const BlackBoardKey key = nextKey++;
			dataMap[key] = block;

			return key;
		}

		template<typename DataType>
		DataType* getItem(BlackBoardKey key) {
			if(dataMap.find(key) == dataMap.end()) {
				return nullptr;
			}

			return reinterpret_cast<DataType*>(dataMap[key]);
		}

		void removeItem(BlackBoardKey key) {
			if(dataMap.find(key) == dataMap.end()) {
				return;
			}

			memoryBlock.free(dataMap[key]);
			dataMap.erase(key);
		}

	private:
	};
}