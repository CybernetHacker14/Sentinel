#ifndef COMMON_MEMORY_MEMFUNCTIONS_H
#define COMMON_MEMORY_MEMFUNCTIONS_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

void Sentinel_MemInit();

void Sentinel_Memcpy(void* dest, const void* src, ULLong n);

void Sentinel_Memset(void* dest, Int val, ULLong n);

#endif  // !COMMON_MEMORY_MEMFUNCTIONS_H
