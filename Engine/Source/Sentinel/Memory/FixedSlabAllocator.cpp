#include "stpch.h"
#include "Sentinel/Memory/FixedSlabAllocator.h"

#include "Sentinel/Common/Core/CompilerDetection.h"
#include "Sentinel/Common/Core/Macros.h"
#include "Sentinel/Common/Core/Assert.h"
#include "Sentinel/Memory/Malloc.h"

#ifdef ST_COMPILER_MSVC
    #include <intrin0.inl.h>
#endif  // ST_COMPILER_MSVC

namespace Sentinel {

    namespace BitUtils {
        static constexpr Int32 MultiplyDeBruijnBitPosition[32] = {0,  1,  28, 2,  29, 14, 24, 3,  30, 22, 20,
                                                                  15, 25, 17, 4,  8,  31, 27, 13, 23, 21, 19,
                                                                  16, 7,  26, 12, 18, 6,  11, 5,  10, 9};

        UInt8 GetFirstTrailingSetBit(UInt32 bits) {
            return MultiplyDeBruijnBitPosition[((UInt32)((bits & (0 - bits)) * 0x077CB531U)) >> 27];
        }

        UInt8 GetFirstTrailingNonSetBit(UInt32 bits) {
            return MultiplyDeBruijnBitPosition[((UInt32)(((~bits) & (0 - (~bits))) * 0x077CB531U)) >> 27];
        }

        UInt8 CountSetBits(UInt32 bits) {
#ifdef ST_COMPILER_MSVC
            return __popcnt(bits);
#else
            bits = bits - ((bits >> 1) & 0x55555555);                     // reuse input as temporary
            bits = (bits & 0x33333333) + ((bits >> 2) & 0x33333333);      // temp
            return ((bits + (bits >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;  // count
#endif  // ST_COMPILER_MSVC
        }

    }  // namespace BitUtils

    template<typename T>
    inline void FixedSlabAllocator<T>::Allocate(UInt8 count) {
        ST_BREAKPOINT_ASSERT(count <= MAX_COUNT, "MAX_COUNT = 32")  // Can only track 32 allocations as of now
        ST_BREAKPOINT_ASSERT(m_FirstAddress == nullptr, "Allocate-Deallocate mismatch")  // Bad allocation

        m_FirstAddress = Calloc(count, sizeof(T));
        m_Occupied = m_Next = 0;
        m_Free = count;
        m_Max = count;
    }

    template<typename T>
    inline void FixedSlabAllocator<T>::Deallocate() {
        ST_BREAKPOINT_ASSERT(
            m_FirstAddress != nullptr, "Allocate-Deallocate mismatch")  // Block should be already allocated

        Free(m_FirstAddress);
        m_FirstAddress = nullptr;
        m_AllocationBitMask = 0;
        m_Occupied = m_Free = m_Max = m_Next = 0;
    }

    template<typename T>
    inline T* FixedSlabAllocator<T>::operator[](const UInt8& index) const {
        T* start = (T*)m_FirstAddress;
        return &(start[index]);
    }

    template<typename T>
    inline const UInt8 FixedSlabAllocator<T>::GetAllocations() const {
        return m_Occupied;
    }

    template<typename T>
    inline const UInt8 FixedSlabAllocator<T>::GetFreeCount() const {
        return m_Free;
    }

    template<typename T>
    template<typename... Args>
    inline T* FixedSlabAllocator<T>::New(Args&&... args) {
        ST_BREAKPOINT_ASSERT(m_Occupied <= m_Max, "Max count reached")

        T* start = (T*)m_FirstAddress;
        T* address = &(start[m_Next]);
        m_AllocationBitMask |= ST_BIT(m_Next);
        m_Next = BitUtils::GetFirstTrailingNonSetBit(m_AllocationBitMask);
        m_Occupied++;
        m_Free--;
        return (new (address) T(ST_FWD(args)...));
    }

    template<typename T>
    template<typename... Args>
    inline T* FixedSlabAllocator<T>::New(UInt8& outIndex, Args&&... args) {
        ST_BREAKPOINT_ASSERT(m_Occupied <= m_Max, "Max count reached")

        outIndex = m_Next;

        T* start = (T*)m_FirstAddress;
        T* address = &(start[m_Next]);
        m_AllocationBitMask |= ST_BIT(m_Next);
        m_Next = BitUtils::GetFirstTrailingNonSetBit(m_AllocationBitMask);
        m_Occupied++;
        m_Free--;
        return (new (address) T(ST_FWD(args)...));
    }

    template<typename T>
    template<typename... Args>
    inline void FixedSlabAllocator<T>::Delete(T* address, Args&&... args) {
        ST_BREAKPOINT_ASSERT(address != nullptr || m_Occupied == 0, "Bad Delete call")

        T* start = (T*)m_FirstAddress;
        UInt8 position = address - start;
        m_AllocationBitMask &= ~(1 << position);
        m_Next = BitUtils::GetFirstTrailingNonSetBit(m_AllocationBitMask);
        m_Occupied--;
        m_Free++;
        address->~T(ST_FWD(args)...);
    }

    template<typename T>
    template<typename... Args>
    inline void FixedSlabAllocator<T>::Delete(UInt8 index, Args&&... args) {
        ST_BREAKPOINT_ASSERT(m_Occupied == 0, "Bad delete call")

        T* start = (T*)m_FirstAddress;
        T* address = &(start[index]);
        m_AllocationBitMask &= ~(1 << index);
        m_Next = BitUtils::GetFirstTrailingNonSetBit(m_AllocationBitMask);
        m_Occupied--;
        m_Free++;
        address->~T(ST_FWD(args)...);
    }

    template<typename T>
    template<typename... Args>
    void FixedSlabAllocator<T>::DeleteAll(Args&&... args) {
        ST_BREAKPOINT_ASSERT(m_Occupied == 0, "Bad delete call")

        for (UInt8 i = 0; i < m_Max; i++) {
            if (m_AllocationBitMask & (1 << i)) Delete(i, ST_FWD(args)...);
        }
    }
}  // namespace Sentinel
