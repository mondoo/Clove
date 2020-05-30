namespace blb::ai {
	template<typename DataType>
	BlackBoard::Key BlackBoard::addItem(DataType item) {
		void* block = memoryBlock.alloc(sizeof(item));
		if(block == nullptr) {
			CLV_LOG_ERROR("{0}: Could not allocate space for new item", CLV_FUNCTION_NAME);
			return INVALID_KEY;
		}

		*reinterpret_cast<DataType*>(block) = item;

		const Key key = nextKey++;
		dataMap[key] = block;

		return key;
	}

	template<typename DataType>
	DataType *BlackBoard::getItem(BlackBoard::Key key)  {
		if(dataMap.find(key) == dataMap.end()) {
			return nullptr;
		}

		return reinterpret_cast<DataType*>(dataMap[key]);
	}
}