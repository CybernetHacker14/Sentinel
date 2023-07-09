#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    template<typename T>
    struct FixedSlabAllocator {
    public:
        void Allocate(UInt8 count);
        void Deallocate();

        template<typename... Args>
        T* New(Args&&... args);

        template<typename... Args>
        T* New(UInt8& outIndex, Args&&... args);

        template<typename... Args>
        void Delete(T* address, Args&&... args);

        template<typename... Args>
        void Delete(UInt8 index, Args&&... args);

        template<typename... Args>
        void DeleteAll(Args&&... args);

        T* operator[](const UInt8& index) const;

        const UInt8 GetAllocations() const;
        const UInt8 GetFreeCount() const;

    private:
        static constexpr UInt8 MAX_COUNT = 32;

    private:
        void* m_FirstAddress = nullptr;  // 8 bytes

        UInt32 m_AllocationBitMask = 0;                           // 4 bytes
        UInt8 m_Occupied = 0, m_Free = 0, m_Max = 0, m_Next = 0;  // 4 bytes
    };
}  // namespace Sentinel
