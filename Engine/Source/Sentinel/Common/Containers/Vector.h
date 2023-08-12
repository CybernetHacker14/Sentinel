#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    template<typename T>
    class Vector {
    public:
        Vector() = default;
        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;

        Vector& operator=(const Vector& other);
        Vector& operator=(Vector&& other) noexcept;

        UInt32 Size() const noexcept { return m_Size; }
        UInt32 Capacity() const noexcept { return m_Capacity; }
        Bool Empty() const noexcept { return m_Size == 0; }

        inline T* Data() noexcept { return m_Data; }
        inline const T* Data() const noexcept { return m_Data; }

        inline T* Begin() noexcept { return m_Data; }
        inline const T* Begin() const noexcept { return m_Data; }

        inline T* End() noexcept { return m_Data + m_Size; }
        inline const T* End() const noexcept { return m_Data + m_Size; }

    private:
        void Reserve(UInt32 count);

    private:
        T* m_Data = nullptr;
        UInt32 m_Size = 0;
        UInt32 m_Capacity = 0;
    };
}  // namespace Sentinel