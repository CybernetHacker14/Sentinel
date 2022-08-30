#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    template<typename T>
    class PoolAllocator {
    public:
        inline void AllocateMemoryBlock(UInt32 maxCount) {
            DeallocateMemoryBlock();

            m_BlockStartingAddress = malloc(sizeof(T) * maxCount);
            m_MaxAllowedAllocations = maxCount;
            m_CurrentAllocations = 0;
            m_FreeList.reserve(maxCount);

            m_AllocatedList.reserve(maxCount);

            DivideBlockIntoChunks();
        }

        inline void DeallocateMemoryBlock() {
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
            m_AllocatedList.emplace_back(m_FreeList[0]);
            std::swap(m_FreeList[0], m_FreeList.back());
            m_FreeList.pop_back();
            std::sort(m_FreeList.begin(), m_FreeList.end());
            std::sort(m_AllocatedList.begin(), m_AllocatedList.end());

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
            m_AllocatedList.emplace_back(m_FreeList[0]);
            std::swap(m_FreeList[0], m_FreeList.back());
            m_FreeList.pop_back();
            std::sort(m_FreeList.begin(), m_FreeList.end());
            std::sort(m_AllocatedList.begin(), m_AllocatedList.end());

            m_CurrentAllocations++;
            return static_cast<T*>(new (address) T(STL::forward<Args>(args)...));
        }

        template<typename U, typename... Args>
        inline T* New(Args&&... args) {
            if (m_CurrentAllocations == m_MaxAllowedAllocations) {
                ST_ENGINE_ASSERT(false, "Max count reached");
                return nullptr;
            }

            static_assert(STL::is_base_of<T, U>::value, "'U' should be a derived from 'T'");

            T* address = m_ChunkAddressMap[m_FreeList[0]];
            m_AllocatedList.emplace_back(m_FreeList[0]);
            std::swap(m_FreeList[0], m_FreeList.back());
            m_FreeList.pop_back();
            std::sort(m_FreeList.begin(), m_FreeList.end());
            std::sort(m_AllocatedList.begin(), m_AllocatedList.end());

            m_CurrentAllocations++;

            return static_cast<T*>(new (address) U(STL::forward<Args>(args)...));
        }

        template<typename U, typename... Args>
        inline T* New(UInt32& outIndex, Args&&... args) {
            if (m_CurrentAllocations == m_MaxAllowedAllocations) {
                ST_ENGINE_ASSERT("Max count reached");
                return nullptr;
            }

            static_assert(STL::is_base_of<T, U>::value, "'U' should be a derived from 'T'");

            outIndex = m_FreeList[0];
            T* address = m_ChunkAddressMap[m_FreeList[0]];
            m_AllocatedList.emplace_back(m_FreeList[0]);
            std::swap(m_FreeList[0], m_FreeList.back());
            m_FreeList.pop_back();
            std::sort(m_FreeList.begin(), m_FreeList.end());
            std::sort(m_AllocatedList.begin(), m_AllocatedList.end());

            m_CurrentAllocations++;
            return static_cast<T*>(new (address) U(STL::forward<Args>(args)...));
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
        STL::unordered_map<UInt32, T*> m_ChunkAddressMap;
        // The key is the address of the chunk. For faster and cleaner reverse access
        STL::unordered_map<T*, UInt32> m_IndexAddressMap;

        STL::vector<UInt32> m_FreeList;
        STL::vector<UInt32> m_AllocatedList;

        UInt32 m_CurrentAllocations = 0;
        UInt32 m_MaxAllowedAllocations;
    };
}  // namespace Sentinel
