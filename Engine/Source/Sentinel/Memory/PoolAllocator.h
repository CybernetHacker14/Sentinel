#pragma once

#include "Sentinel/Common/Core/Malloc.h"
#include "Sentinel/Common/Containers/Vector.h"
#include <sparse_map.h>

namespace Sentinel {
    template<typename T>
    class PoolAllocator {
    public:
        inline void AllocateMemoryBlock(UInt8 maxCount) {
            ST_ENGINE_ASSERT(m_BlockStartingAddress == nullptr, "Bad Allocation");

            m_BlockStartingAddress = Calloc(maxCount, sizeof(T));

            m_MaxAllowedAllocations = maxCount;
            m_CurrentAllocations = 0;
            m_FreeList.reserve(maxCount);
            m_AllocatedList.reserve(maxCount);

            DivideBlockIntoChunks();
        }

        inline void DeallocateMemoryBlock() {
            if (m_BlockStartingAddress != nullptr) {
                Free(m_BlockStartingAddress);
                m_BlockStartingAddress = nullptr;
                m_CurrentAllocations = 0;
            }
        }

        inline void DivideBlockIntoChunks() {
            for (UInt32 i = 0; i < m_MaxAllowedAllocations; i++) {
                T* startPtr = (T*)m_BlockStartingAddress;
                m_ChunkAddressMap[i] = &(startPtr[i]);
                m_IndexAddressMap[m_ChunkAddressMap[i]] = i;
                m_FreeList.emplace_back(i);
            }
        }

        template<typename... Args>
        inline T* New(Args&&... args) {
            if (m_CurrentAllocations == m_MaxAllowedAllocations) {
                ST_ENGINE_ASSERT(false, "Max count reached");
                return nullptr;
            }

            T* address = m_ChunkAddressMap[m_FreeList[0]];
            m_AllocatedList.emplace_back(m_FreeList[0]);
            std::swap(m_FreeList[0], m_FreeList.back());
            m_FreeList.pop_back();
            std::sort(m_FreeList.begin(), m_FreeList.end());
            std::sort(m_AllocatedList.begin(), m_AllocatedList.end());

            m_CurrentAllocations++;

            return (new (address) T(STL::forward<Args>(args)...));
        }

        template<typename... Args>
        inline T* New(UInt32& outIndex, Args&&... args) {
            if (m_CurrentAllocations == m_MaxAllowedAllocations) {
                ST_ENGINE_ASSERT(false, "Max count reached");
                return nullptr;
            }

            outIndex = m_FreeList[0];
            T* address = m_ChunkAddressMap[m_FreeList[0]];
            m_AllocatedList.emplace_back(m_FreeList[0]);
            std::swap(m_FreeList[0], m_FreeList.back());
            m_FreeList.pop_back();
            std::sort(m_FreeList.begin(), m_FreeList.end());
            std::sort(m_AllocatedList.begin(), m_AllocatedList.end());

            m_CurrentAllocations++;
            return static_cast<T*>(new (address) T(STL::forward<Args>(args)...));
        }

        template<typename... Args>
        inline void Delete(T* address, Args&&... args) {
            if (STL::find(m_FreeList.begin(), m_FreeList.end(), m_IndexAddressMap[address]) != m_FreeList.end()) {
                ST_ENGINE_ASSERT(false, "Memory address is free and part of free list");
                return;
            }

            m_FreeList.emplace_back(m_IndexAddressMap[address]);
            STL::remove(m_AllocatedList.begin(), m_AllocatedList.end(), m_IndexAddressMap[address]);
            std::sort(m_FreeList.begin(), m_FreeList.end());
            address->~T(STL::forward<Args>(args)...);
        }

        template<typename... Args>
        inline void DeleteAll(Args&&... args) {
            for (UInt32 index: m_AllocatedList) {
                m_ChunkAddressMap[index]->~T(STL::forward<Args>(args)...);
                m_FreeList.emplace_back(index);
            }
            std::sort(m_FreeList.begin(), m_FreeList.end());
            m_AllocatedList.clear();
            m_AllocatedList.reserve(m_MaxAllowedAllocations);
        }

        inline T* operator[](const UInt32& index) { return m_ChunkAddressMap[index]; }

        inline const UInt32 GetTotalAllocations() { return m_CurrentAllocations; }
        inline const UInt32 GetFreeCount() { return m_FreeList.size(); }

    private:
        void* m_BlockStartingAddress = nullptr;

        // The key is the supposed index of the block
        tsl::sparse_map<UInt32, T*> m_ChunkAddressMap;
        // The key is the address of the chunk. For faster and cleaner reverse access
        tsl::sparse_map<T*, UInt32> m_IndexAddressMap;

        Vector<UInt32> m_FreeList;
        Vector<UInt32> m_AllocatedList;

        UInt32 m_CurrentAllocations = 0;
        UInt32 m_MaxAllowedAllocations = 0;

        UInt32 m_0_31_AllocationBitMask = 0;
        UInt32 m_32_63_AllocationBitMask = 0;
        UInt32 m_64_95_AllocationBitMask = 0;
        UInt32 m_96_127_AllocationBitMask = 0;
        UInt32 m_128_159_AllocationBitMask = 0;
        UInt32 m_160_191_AllocationBitMask = 0;
        UInt32 m_192_223_AllocationBitMask = 0;
        UInt32 m_224_255_AllocationBitMask = 0;
    };
}  // namespace Sentinel
