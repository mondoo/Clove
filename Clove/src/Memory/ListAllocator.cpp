#include "Clove/Memory/ListAllocator.hpp"

#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>

namespace clv::mem {
    ListAllocator::ListAllocator(size_t sizeBytes)
        : listSize(sizeBytes)
        , freeMemory(true) {
#if CLV_ENABLE_MEMORY_DEBUGGING
        GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Constructing new ListAllocator. Size {0}. ", listSize);
#endif
        rawList = reinterpret_cast<std::byte*>(malloc(listSize));
        head    = rawList;
    }

    ListAllocator::ListAllocator(std::byte* start, size_t sizeBytes)
        : listSize(sizeBytes)
        , freeMemory(false) {
        rawList = reinterpret_cast<std::byte*>(start);
        head    = rawList;
    }

    ListAllocator::ListAllocator(ListAllocator&& other) noexcept {
        rawList  = other.rawList;
        listSize = other.listSize;
        head     = other.head;

        list = std::move(list);

        freeMemory       = other.freeMemory;
        other.freeMemory = false;

#if GARLIC_DEBUG
        allocations = other.allocations;
        frees       = other.frees;
#endif
    }

    ListAllocator& ListAllocator::operator=(ListAllocator&& other) noexcept {
        rawList  = other.rawList;
        listSize = other.listSize;
        head     = other.head;

        list = std::move(list);

        freeMemory       = other.freeMemory;
        other.freeMemory = false;

#if GARLIC_DEBUG
        allocations = other.allocations;
        frees       = other.frees;
#endif
        return *this;
    }

    ListAllocator::~ListAllocator() {
        if(freeMemory) {
#if GARLIC_DEBUG
            if(allocations > frees) {
                GARLIC_LOG(garlicLogContext, Log::Level::Warning, "List Allocator destructed with active memory. Block will be freed but destructors will not be called on occupying elements");
            }
#endif
            ::free(rawList);
        }
    }

    void* ListAllocator::alloc(size_t bytes) {
        Header* header = nullptr;
        for(auto* freeHeader : list) {//Finding first available
            if(bytes <= freeHeader->blockSize) {
                header = freeHeader;
                list.remove(header);
                break;
                //TODO: Can reduce fragmentation by putting remaining bytes back in the list
            }
        }

        if(header == nullptr) {
            if((head - rawList) + bytes > listSize) {
                GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: Not enough space left to allocate {1} bytes.", GARLIC_FUNCTION_NAME_PRETTY, bytes);
                return nullptr;
            }

            header            = reinterpret_cast<Header*>(head);
            header->blockSize = bytes;
            head += sizeof(Header) + bytes;
        }
#if GARLIC_DEBUG
        ++allocations;
#endif

        return reinterpret_cast<char*>(header) + sizeof(Header);
    }

    void ListAllocator::free(void* ptr) {
        std::byte* data = reinterpret_cast<std::byte*>(ptr);
        Header* header  = reinterpret_cast<Header*>(data - sizeof(Header));

        list.push_back(header);
#if GARLIC_DEBUG
        ++frees;
#endif
    }
}
