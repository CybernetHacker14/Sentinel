#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/PlatformDetection.h"
#include "Sentinel/Common/Core/STL.h"

namespace Sentinel {
    struct TestStruct {
    public:
        STL::string testString;
        Float testFloat;
        UInt32 testInt;
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
            if (m_BlockStartingAddress != nullptr) {
                free(m_BlockStartingAddress);
                m_BlockStartingAddress = nullptr;
            }
        }

    public:
        void* m_BlockStartingAddress = nullptr;

        UInt32 m_CurrentAllocations = 0;
        UInt32 m_TotalDeallocations;
        UInt32 m_MaxAllocationCount;
    };
}  // namespace Sentinel
