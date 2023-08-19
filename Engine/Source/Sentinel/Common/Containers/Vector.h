#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    template<typename T>
    class Vector {
    public:
        static constexpr UInt8 GROW_FACTOR = 2;

    public:
        Vector() = default;
        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;

        Vector(const std::initializer_list<T>& list);

        Vector& operator=(const Vector& other);
        Vector& operator=(Vector&& other) noexcept;

        ~Vector();

        T& operator[](UInt32 pos);
        const T& operator[](UInt32 pos) const;

        void Clear() noexcept;

        UInt32 Size() const noexcept { return m_Size; }
        UInt32 Capacity() const noexcept { return m_Capacity; }
        Bool Empty() const noexcept { return m_Size == 0; }

        inline T* Data() noexcept { return m_Data; }
        inline const T* Data() const noexcept { return m_Data; }

        inline T* Begin() noexcept { return m_Data; }
        inline const T* Begin() const noexcept { return m_Data; }

        inline T* End() noexcept { return m_Data + m_Size; }
        inline const T* End() const noexcept { return m_Data + m_Size; }

        void Push_Back(const T& value);
        void Push_Back(T&& value);

        template<class... Args>
        void Emplace_Back(Args&&... args);

        void Pop_Back();

    private:
        void Reserve(UInt32 count);

    private:
        T* m_Data = nullptr;
        UInt32 m_Size = 0;
        UInt32 m_Capacity = 0;
    };
}  // namespace Sentinel