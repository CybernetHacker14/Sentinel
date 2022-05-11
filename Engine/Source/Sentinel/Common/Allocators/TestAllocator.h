#pragma once

#include "Sentinel/Common/Core/STL.h"
#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/PlatformDetection.h"

namespace Sentinel
{
	struct TestStruct {
	public:
		UInt32 testInt;
		STL::string testString;
		Float testFloat;
	};

	template<typename T>
	class TestAllocator {
	public:
		inline void AllocateMemoryBlock(UInt32 maxCount) {
			Clear();

			m_MaxAllocationCount = maxCount;
			m_BlockStartingAddress = malloc(sizeof(T) * maxCount);
		}

		inline void Clear() {
			if (m_BlockStartingAddress != nullptr)
				free(*m_BlockStartingAddress);
		}

	public:
		void* m_BlockStartingAddress;

		UInt32 m_CurrentAllocations;
		UInt32 m_TotalDeallocations;
		UInt32 m_MaxAllocationCount;
	};
}
