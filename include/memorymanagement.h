#ifndef __CPOS__MEMORYMANAGEMENT_
#define __CPOS__MEMORYMANAGEMENT_

#include "common/types.h"

namespace cpos {
    struct MemoryChunk {
        MemoryChunk* next;
        MemoryChunk* prev;
        bool allocated;
        common::size_t size;
    };

    class MemoryManager {
    public:
        MemoryManager(common::size_t first, common::size_t size);
        ~MemoryManager();

        void* malloc(common::size_t size);
        void free(void* ptr);

        static MemoryManager* activeMemoryManager;
    private:
        MemoryChunk* first;
    };
}

void* operator new(cpos::common::size_t size);
void* operator new[](cpos::common::size_t size);

void* operator new(cpos::common::size_t size, void* ptr);
void* operator new[](cpos::common::size_t size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);

#endif