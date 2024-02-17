#ifndef ALLOCATOR_FIXEDSLABALLOCATOR_H
#define ALLOCATOR_FIXEDSLABALLOCATOR_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct FixedSlabAllocator {
    void* startingAddress;
    ULLong itemSize;
    ULLong allocationBitMask;
    UShort max, next, occupied, free;
} FixedSlabAllocator;

void Sentinel_FixedSlabAllocator_Allocate(FixedSlabAllocator* allocator, ULLong itemSize, UShort count);

void* Sentinel_FixedSlabAllocator_New(FixedSlabAllocator* allocator, UShort* outIndex);

void Sentinel_FixedSlabAllocator_Delete(FixedSlabAllocator* allocator, void* address, UShort* outIndex);

void Sentinel_FixedSlabAllocator_DeleteAll(FixedSlabAllocator* allocator);

void Sentinel_FixedSlabAllocator_Deallocate(FixedSlabAllocator* allocator);

#endif  // !ALLOCATOR_FIXEDSLABALLOCATOR_H
