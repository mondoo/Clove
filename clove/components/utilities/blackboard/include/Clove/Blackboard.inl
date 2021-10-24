#include <Clove/Log/Log.hpp>

CLOVE_DECLARE_LOG_CATEGORY(CloveBlackboard)

namespace clove {
    template<typename DataType>
    void BlackBoard::setValue(Key key, DataType value) {
        void *block{ nullptr };

        if(dataMap.find(key) == dataMap.end()) {
            block = memoryBlock.alloc(sizeof(DataType), alignof(DataType));
            if(block == nullptr) {
                CLOVE_LOG(CloveBlackboard, LogLevel::Error, "{0}: Could not allocate space for new item", CLOVE_FUNCTION_NAME);
                return;
            }

            dataMap[key] = block;
        } else {
            block = dataMap.at(key);
            
            //About to override the value so make sure it's dtor has been called.
            reinterpret_cast<DataType *>(block)->~DataType();
        }

        new(block) DataType{ std::move(value) };
    }

    template<typename DataType>
    std::optional<DataType> BlackBoard::getValue(Key key) const {
        if(dataMap.find(key) == dataMap.end()) {
            return {};
        }

        return { *reinterpret_cast<DataType *>(dataMap.at(key)) };
    }
}