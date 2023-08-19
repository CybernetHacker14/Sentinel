#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    inline void* Malloc(Size_t size);
    inline void* Calloc(Size_t count, Size_t size);
    inline void* Realloc(void* ptr, Size_t size);
    inline void Free(void* ptr);
}  // namespace Sentinel