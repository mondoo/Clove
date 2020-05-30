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
		BlackBoard();
		BlackBoard(size_t memorySize);

		BlackBoard(const BlackBoard& other) = delete;
		BlackBoard(BlackBoard&& other) noexcept;

		BlackBoard& operator=(const BlackBoard& other) = delete;
		BlackBoard& operator=(BlackBoard&& other) noexcept;

		~BlackBoard();

		template<typename DataType>
		Key addItem(DataType item);

		template<typename DataType>
		DataType* getItem(Key key);

		void removeItem(Key key);
	};
}

#include "BlackBoard.inl"