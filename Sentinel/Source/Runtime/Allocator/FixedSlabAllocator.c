#include "stpch.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Common/Core/CompilerDetection.h"
#include "Common/Core/Macros.h"

#ifdef ST_DEBUG
    #include "Common/Core/Assert.h"
#endif  // ST_DEBUG

#ifdef ST_COMPILER_MSVC
    #include <intrin.h>
#endif  // ST_COMPILER_MSVC

static const UChar MultiplyDeBruijnBitPosition[64] = {0,  1,  2,  53, 3,  7,  54, 27, 4,  38, 41, 8,  34, 55, 48, 28,
                                                      62, 5,  39, 46, 44, 42, 22, 9,  24, 35, 59, 56, 49, 18, 29, 11,
                                                      63, 52, 6,  26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
                                                      51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12};

static UShort GetFirstTrailingNonSetBit(ULLong bits) {
    return MultiplyDeBruijnBitPosition[(((~bits) & (0 - (~bits))) * 0x022FDD63CC95386D) >> 58];
}

static UShort CountSetBits(ULLong bits) {
#ifdef ST_COMPILER_MSVC
    return (UShort)__popcnt64(bits);
#else
    bits -= (bits >> 1) & 0x5555555555555555UL;
    bits = (bits & 0x3333333333333333UL) + ((bits >> 2) & 0x3333333333333333UL);
    return (UShort)((((bits + (bits >> 4)) & 0xF0F0F0F0F0F0F0FUL) * 0x101010101010101UL) >> 56);
#endif  // ST_COMPILER_MSVC
}

void Sentinel_FixedSlabAllocator_Allocate(FixedSlabAllocator* allocator, ULLong itemSize, UShort count) {
#ifdef ST_DEBUG
    ST_BREAKPOINT_ASSERT(count <= 64, "Max Count > 64");
#endif  // ST_DEBUG

    allocator->allocationBitMask = 0;
    allocator->free = allocator->max = count;
    allocator->next = allocator->occupied = 0;

    allocator->itemSize = itemSize;

    allocator->startingAddress = Sentinel_Malloc((ULLong)(itemSize * count));
}

void* Sentinel_FixedSlabAllocator_New(FixedSlabAllocator* allocator, UShort* outIndex) {
    void* addr = (Char*)allocator->startingAddress + (allocator->next * allocator->itemSize);

    allocator->allocationBitMask |= ST_BIT64(allocator->next);
    *outIndex = allocator->next;
    allocator->next = GetFirstTrailingNonSetBit(allocator->allocationBitMask);
    allocator->occupied++;
    allocator->free--;

    return addr;
}

void Sentinel_FixedSlabAllocator_Delete(FixedSlabAllocator* allocator, void* address) {
    Sentinel_Memset(address, 0, allocator->itemSize);

    UShort position = (UShort)(((Char*)allocator->startingAddress - (Char*)address) / allocator->itemSize);
    allocator->allocationBitMask &= ~(1ull << position);
    allocator->next = GetFirstTrailingNonSetBit(allocator->allocationBitMask);
    allocator->occupied--;
    allocator->free++;
}

void Sentinel_FixedSlabAllocator_DeleteAll(FixedSlabAllocator* allocator) {
    Sentinel_Memset(allocator->startingAddress, 0, (ULLong)(allocator->itemSize * allocator->max));
    allocator->occupied = allocator->next = 0;
    allocator->free = allocator->max;
    allocator->allocationBitMask = 0;
}

void Sentinel_FixedSlabAllocator_Deallocate(FixedSlabAllocator* allocator) {
    Sentinel_Free(allocator->startingAddress);

    allocator->allocationBitMask = 0;
    allocator->itemSize = 0;
    allocator->free = allocator->max = allocator->next = allocator->occupied = 0;
}
