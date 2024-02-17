#include "stpch.h"
#include "Common/Memory/Malloc.h"

#include <Windows.h>
#include <stdint.h>

#if INTPTR_MAX == 0x7FFFFFFFFFFFFFFFLL
    #define PTR_SIZE 8
#elif INTPTR_MAX == 0x7FFFFFFF
    #define PTR_SIZE 4
#else
    #define PTR_SIZE 0
    #error "Unsupported Platform"
#endif  // INTPTR_MAX == 0x7FFFFFFFFFFFFFFFLL

#define MIN_ALLOC_ALIGNMENT       (PTR_SIZE * 2)
#define ALIGN_TO(size, alignment) (((size) + (alignment)-1) & ~((alignment)-1))

static ULLong totalDynamicAllocations = 0;

void* Sentinel_Malloc(ULLong size) {
    ++totalDynamicAllocations;
    return (void*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
}

void Sentinel_Free(void* ptr) {
    HeapFree(GetProcessHeap(), 0, ptr);
    --totalDynamicAllocations;
}

ULLong Sentinel_GetTotalDynamicAllocations() {
    return totalDynamicAllocations;
}
