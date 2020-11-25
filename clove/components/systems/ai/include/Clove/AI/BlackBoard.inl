#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    template<typename DataType>
    void BlackBoard::setValue(Key key, DataType value) {
        if(dataMap.find(key) == dataMap.end()) {
            void *block = memoryBlock.alloc(sizeof(DataType));
            if(block == nullptr) {
                GARLIC_LOG(garlicLogContext, LogLevel::Error, "{0}: Could not allocate space for new item", GARLIC_FUNCTION_NAME);
                return;
            }

            dataMap[key] = block;
        }

        *reinterpret_cast<DataType *>(dataMap[key]) = value;
    }

    template<typename DataType>
    std::optional<DataType> BlackBoard::getValue(Key key) {
        if(dataMap.find(key) == dataMap.end()) {
            return {};
        }

        return { *reinterpret_cast<DataType *>(dataMap[key]) };
    }
}