#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/Assert.h"
#include "Sentinel/Common/Core/STL.h"

namespace Sentinel {
    template<typename T>
    class PoolAllocator {
    public:
        enum class AllocationState { ALLOCATED = 0, FREE = 1 };

    public:
        inline void AllocateMemoryBlock(UInt32 maxCount) {
            ClearMemoryBlock();

            m_BlockStartingAddress = malloc(sizeof(T) * maxCount);
            m_MaxAllowedAllocations = maxCount;
            m_CurrentAllocations = 0;
            m_FreeList.reserve(maxCount);

            DivideBlockIntoChunks();
        }

        inline void ClearMemoryBlock() {
            if (m_BlockStartingAddress != nullptr) {
                free(m_BlockStartingAddress);
                m_BlockStartingAddress = nullptr;
                m_CurrentAllocations = 0;
            }
        }

        inline void DivideBlockIntoChunks() {
            for (UInt32 i = 0; i < m_MaxAllowedAllocations; i++) {
                m_ChunkAddressMap[i] = static_cast<T*>(((T*)m_BlockStartingAddress) + (i * sizeof(T)));
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
            std::swap(m_FreeList[0], m_FreeList.back());
            m_FreeList.pop_back();
            std::sort(m_FreeList.begin(), m_FreeList.end());

            m_CurrentAllocations++;

            return static_cast<T*>(new (address) T(STL::forward<Args>(args)...));
        }

        template<typename... Args>
        inline T* New(UInt32& outIndex, Args&&... args) {
            if (m_CurrentAllocations == m_MaxAllowedAllocations) {
                ST_ENGINE_ASSERT("Max count reached");
                return nullptr;
            }

            outIndex = m_FreeList[0];
            T* address = m_ChunkAddressMap[m_FreeList[0]];
            std::swap(m_FreeList[0], m_FreeList.back());
            m_FreeList.pop_back();
            std::sort(m_FreeList.begin(), m_FreeList.end());
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
            std::sort(m_FreeList.begin(), m_FreeList.end());
            address->~T(STL::forward<Args>(args)...);
        }

        inline T* operator[](const UInt32& index) { return m_ChunkAddressMap[index]; }

    private:
        void* m_BlockStartingAddress = nullptr;

        // The key is the supposed index of the block
        STL::unordered_map<UInt32, T*> m_ChunkAddressMap;
        // The key is the address of the chunk. For faster and cleaner reverse access
        STL::unordered_map<T*, UInt32> m_IndexAddressMap;

        STL::vector<UInt32> m_FreeList;

        UInt32 m_CurrentAllocations = 0;
        UInt32 m_MaxAllowedAllocations;
    };
}  // namespace Sentinel
