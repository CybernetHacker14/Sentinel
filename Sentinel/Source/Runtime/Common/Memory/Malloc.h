#ifndef COMMON_MEMORY_MALLOC_H
#define COMMON_MEMORY_MALLOC_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

void* Sentinel_Malloc(ULLong size);

void Sentinel_Free(void* ptr);

ULLong Sentinel_GetTotalDynamicAllocations();

#endif  // !COMMON_MEMORY_MALLOC_H
