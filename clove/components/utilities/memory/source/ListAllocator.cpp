#include "Clove/Memory/ListAllocator.hpp"

#include <Clove/Log/Log.hpp>
#include <cstdlib>

namespace clove {
    ListAllocator::ListAllocator(size_t sizeBytes)
        : listSize(sizeBytes) {
        rawList = reinterpret_cast<std::byte *>(malloc(listSize));
        head    = rawList;
    }

    ListAllocator::ListAllocator(std::byte *start, size_t sizeBytes)
        : listSize(sizeBytes)
        , freeMemory(false) {
        rawList = reinterpret_cast<std::byte *>(start);
        head    = rawList;
    }

    ListAllocator::ListAllocator(ListAllocator &&other) noexcept
        : rawList{ other.rawList }
        , listSize{ other.listSize }
        , head{ other.head }
        , freeList{ std::move(other.freeList) }
        , freeMemory{ other.freeMemory } {
        other.freeMemory = false;
    }

    ListAllocator &ListAllocator::operator=(ListAllocator &&other) noexcept {
        rawList    = other.rawList;
        listSize   = other.listSize;
        head       = other.head;
        freeList       = std::move(freeList);
        freeMemory = other.freeMemory;

        other.freeMemory = false;

        return *this;
    }

    ListAllocator::~ListAllocator() {
        if(freeMemory) {
            ::free(rawList);
        }
    }

    void *ListAllocator::alloc(size_t size, size_t alignment) {
        size_t const totalAllocationSize{ size + alignment };

        Header *header{ nullptr };
        for(auto *freeHeader : freeList) {
            if(freeHeader->blockSize >= totalAllocationSize) {
                header = freeHeader;
                freeList.remove(header);
                break;
                //TODO: Can reduce fragmentation by putting remaining bytes back in the list
            }
        }

        if(header == nullptr) {
            if((head - rawList) + sizeof(Header) + totalAllocationSize > listSize) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Not enough space left to allocate {1} bytes.", CLOVE_FUNCTION_NAME_PRETTY, size);
                return nullptr;
            }

            header            = reinterpret_cast<Header *>(head);
            header->blockSize = totalAllocationSize;
            head += sizeof(Header) + totalAllocationSize;
        }

        //Get a pointer to the piece of memory after the header. Cast to uint8_t so the + operator knows how many bytes to advance by.
        uint8_t *memory{ reinterpret_cast<uint8_t *>(header) + sizeof(Header) };

        size_t const remainingAlignment{ alignment != 0 ? reinterpret_cast<uintptr_t>(memory) % alignment : 0 };
        size_t const alignmentOffset{ remainingAlignment != 0 ? alignment - remainingAlignment : 0 };
        return memory + alignmentOffset;
    }

    void ListAllocator::free(void *ptr) {
        std::byte *data{ reinterpret_cast<std::byte *>(ptr) };
        Header *header{ reinterpret_cast<Header *>(data - sizeof(Header)) };

        freeList.push_back(header);
    }
}
