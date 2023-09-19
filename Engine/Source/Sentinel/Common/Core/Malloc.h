#include "stpch.h"
#include "Sentinel/Common/Core/CompilerDetection.h"
#include "Sentinel/Common/Strings/MemFunctions.h"

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

namespace Sentinel {
    static void* Malloc(Size_t size) {
#ifdef ST_COMPILER_MSVC
        return _aligned_malloc(size, MIN_ALLOC_ALIGNMENT);
#else
        return malloc(size);
#endif  // ST_COMPILER_MSVC
    }

    static void* Calloc(Size_t count, Size_t size) {
#ifdef ST_COMPILER_MSVC
        Size_t sz = count * size;
        void* ptr = Malloc(sz);
        MemFunctions::Memset(ptr, 0, sz);
#else
        void* ptr = calloc(count, size);
#endif  // ST_COMPILER_MSVC
        return ptr;
    }

    static void* Realloc(void* ptr, Size_t size) {
#ifdef ST_COMPILER_MSVC
        return _aligned_realloc(ptr, size, MIN_ALLOC_ALIGNMENT);
#else
        return realloc(ptr, size);
#endif  // ST_COMPILER_MSVC
    }

    static void Free(void* ptr) {
#ifdef ST_COMPILER_MSVC
        _aligned_free(ptr);
#else
        free(ptr);
#endif  // ST_COMPILER_MSVC
    }
}  // namespace Sentinel