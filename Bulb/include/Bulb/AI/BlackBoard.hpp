#pragma once

#include <Clove/Memory/ListAllocator.hpp>

namespace blb::ai {
	class BlackBoard {
		//TYPES
	public:
		using Key = size_t;
		
		//VARIABLES
	private:
		static inline constexpr Key INVALID_KEY = 0;

		clv::mem::ListAllocator memoryBlock;
		Key nextKey = 1;

		std::map<Key, void*> dataMap;

		//FUNCTIONS
	public:
		//TODO: Ctors
		BlackBoard() 
			: memoryBlock(1024 * 1024) { //Lets just give ourselves a meg at the moment
		}

		~BlackBoard() = default;

		template<typename DataType>
		Key addItem(DataType item) {
			void* block = memoryBlock.alloc(sizeof(item));
			if(block == nullptr) {
				//TODO: Error
			}

			*reinterpret_cast<DataType*>(block) = item;

			const Key key = nextKey++;
			dataMap[key] = block;

			return key;
		}

		template<typename DataType>
		DataType* getItem(Key key) {
			if(dataMap.find(key) == dataMap.end()) {
				return nullptr;
			}

			return reinterpret_cast<DataType*>(dataMap[key]);
		}

		void removeItem(Key key) {
			if(dataMap.find(key) == dataMap.end()) {
				return;
			}

			memoryBlock.free(dataMap[key]);
			dataMap.erase(key);
		}

	private:
	};
}