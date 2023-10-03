#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/Malloc.h"

#include <initializer_list>

// Reference: https://github.com/sigerror/fast-vector

namespace Sentinel {
    namespace VectorUtils {
        template<class T>
        inline void construct_range(T* begin, T* end) {
            while (begin != end) {
                new (begin) T;
                begin++;
            }
        }

        template<class T>
        inline void copy_range(T* begin, T* end, T* dest) {
            while (begin != end) {
                new (dest) T(*begin);
                begin++;
                dest++;
            }
        }

        template<class T>
        inline void destruct_range(T* begin, T* end) {
            while (begin != end) {
                begin->~T();
                begin++;
            }
        }
    }  // namespace VectorUtils

    template<typename T>
    class Vector {
    public:
        static constexpr UInt8 GROW_FACTOR = 2;

    public:
        Vector() = default;

        inline Vector(const Vector& other) : m_Size(other.m_Size), m_Capacity(other.m_Capacity) {
            m_Data = (T*)Malloc(sizeof(T) * other.m_Capacity);

            std::is_trivial<T>::value ? MemFunctions::Memcpy(m_Data, other.m_Data, other.m_Size)
                                      : VectorUtils::copy_range(other.Begin(), other.End(), m_Data);

            return *this;
        }

        inline Vector(Vector&& other) noexcept
            : m_Data(other.m_Data), m_Size(other.m_Size), m_Capacity(other.m_Capacity) {
            other.m_Data = nullptr;
        }

        inline Vector(const std::initializer_list<T>& list) {
            m_Data = (T*)Malloc(sizeof(T) * list.size());
            m_Size = list.size();
            VectorUtils::copy_range((T*)(list.begin()), (T*)(list.end()), m_Data);
        }

        inline Vector& operator=(const Vector& other) {
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;

            m_Data = (T*)Malloc(sizeof(T) * other.m_Capacity);
            std::is_trivial<T>::value ? MemFunctions::Memcpy(m_Data, other.m_Data, other.m_Size)
                                      : VectorUtils::copy_range(other.Begin(), other.End(), m_Data);

            return *this;
        }

        inline Vector& operator=(Vector&& other) noexcept {
            m_Data = other.m_Data;
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;

            return *this;
        }

        inline ~Vector() {
            if (!std::is_trivial<T>::value) VectorUtils::destruct_range(Begin(), End());
            Free(m_Data);
        }

        inline T& operator[](UInt32 pos) {
            // ST_STATIC_ASSERT(pos < m_Size);
            return m_Data[pos];
        }

        inline const T& operator[](UInt32 pos) const {
            // ST_STATIC_ASSERT(pos < m_Size);
            return m_Data[pos];
        }

        inline void Clear() noexcept {
            if (!std::is_trivial<T>::value) VectorUtils::destruct_range(Begin(), End());
            m_Size = 0;
        }

        UInt32 Size() const noexcept { return m_Size; }
        UInt32 Capacity() const noexcept { return m_Capacity; }
        Bool Empty() const noexcept { return m_Size == 0; }

        inline T* Data() noexcept { return m_Data; }
        inline const T* Data() const noexcept { return m_Data; }

        inline T* Begin() noexcept { return m_Data; }
        inline const T* Begin() const noexcept { return m_Data; }

        inline T* End() noexcept { return m_Data + m_Size; }
        inline const T* End() const noexcept { return m_Data + m_Size; }

        inline void Push_Back(const T& value) {
            if (m_Size == m_Capacity) Reserve(m_Capacity * GROW_FACTOR + 1);
            if (std::is_trivial<T>::value)
                m_Data[m_Size] = value;
            else
                new (m_Data + m_Size) T(value);

            m_Size++;
        }

        inline void Push_Back(T&& value) {
            if (m_Size == m_Capacity) Reserve(m_Capacity * GROW_FACTOR + 1);
            if (std::is_trivial<T>::value)
                m_Data[m_Size] = value;
            else
                new (m_Data + m_Size) T(ST_MOV(value));

            m_Size++;
        }

        template<class... Args>
        inline void Emplace_Back(Args&&... args) {
            // Use "Push_Back()" instead of "Emplace_Back()" for trivial types
            // ST_STATIC_ASSERT(!std::is_trivial<T>::value);

            if (m_Size == m_Capacity) Reserve(m_Capacity * GROW_FACTOR + 1);

            new (m_Data + m_Size) T(ST_MOV(args)...);

            m_Size++;
        }

        inline void Pop_Back() {
            // ST_STATIC_ASSERT(m_Size > 0);  // Container is empty

            if (!std::is_trivial<T>::value) m_Data[m_Size - 1].~T();

            m_Size--;
        }

    private:
        inline void Reserve(UInt32 count) {
            // ST_STATIC_ASSERT(count > m_Capacity);

            if (std::is_trivial<T>::value) {
                m_Data = (T*)Realloc(m_Data, sizeof(T) * count);
                // ST_STATIC_ASSERT(m_Data != nullptr);

            } else {
                T* temp = (T*)Malloc(sizeof(T) * count);
                // ST_STATIC_ASSERT(m_Data != nullptr);

                VectorUtils::copy_range(Begin(), End(), temp);
                VectorUtils::destruct_range(Begin(), End());

                Free(m_Data);

                m_Data = temp;
            }

            m_Capacity = count;
        }

    private:
        T* m_Data = nullptr;
        UInt32 m_Size = 0;
        UInt32 m_Capacity = 0;
    };
}  // namespace Sentinel