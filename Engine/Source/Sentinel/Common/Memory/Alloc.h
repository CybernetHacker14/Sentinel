#pragma once

#include "Sentinel/Common/Core/STL.h"
#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel
{
	class Alloc {
	public:

		template<typename T>
		inline static void* Allocate(UInt32 count) {
			return malloc(count * sizeof(T));
		}

		inline static void Free(UInt32 index) {

		}

		inline static void* AllocateAligned(UInt64 size, UInt64 alignment) {
			return _aligned_malloc(size, alignment);
		}

		inline static void Deallocate(void* block) {
			free(block);
		}

		inline static void DellocateAligned(void* block) {
			_aligned_free(block);
		}
	};
}
