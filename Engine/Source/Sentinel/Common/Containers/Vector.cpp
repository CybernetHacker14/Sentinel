#include "stpch.h"
#include "Sentinel/Common/Containers/Vector.h"
#include "Sentinel/Common/Core/Malloc.h"
#include "Sentinel/Common/Core/Assert.h"

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
    Vector<T>::Vector(const Vector& other) : m_Size(other.m_Size), m_Capacity(other.m_Capacity) {
        m_Data = (T*)Malloc(sizeof(T) * other.m_Capacity);

        std::is_trivial<T>::value ? MemFunctions::Memcpy(m_Data, other.m_Data, other.m_Size)
                                  : VectorUtils::copy_range(other.Begin(), other.End(), m_Data);

        return *this;
    }

    template<typename T>
    Vector<T>::Vector(Vector&& other) noexcept
        : m_Data(other.m_Data), m_Size(other.m_Size), m_Capacity(other.m_Capacity) {
        other.m_Data = nullptr;
    }

    template<typename T>
    Vector<T>::Vector(const std::initializer_list<T>& list) : m_Data(list) {
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector& other) {
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        m_Data = (T*)Malloc(sizeof(T) * other.m_Capacity);

        std::is_trivial<T>::value ? MemFunctions::Memcpy(m_Data, other.m_Data, other.m_Size)
                                  : VectorUtils::copy_range(other.Begin(), other.End(), m_Data);

        return *this;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
        m_Data = other.m_Data;
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        return *this;
    }

    template<typename T>
    Vector<T>::~Vector() {
        (!std::is_trivial<T>::value) ? VectorUtils::destruct_range(Begin(), End()) ? Free(m_Data);
    }

    template<typename T>
    T& Vector<T>::operator[](UInt32 pos) {
        ST_STATIC_ASSERT(pos < m_Size);
        return m_Data[pos];
    }

    template<typename T>
    const T& Vector<T>::operator[](UInt32 pos) const {
        ST_STATIC_ASSERT(pos < m_Size);
        return m_Data[pos];
    }

    template<typename T>
    void Vector<T>::Clear() noexcept {
        if constexpr (!std::is_trivial<T>::value) destruct_range(Begin(), End());
        m_Size = 0;
    }

    template<typename T>
    void Vector<T>::Push_Back(const T& value) {
        if (m_Size == m_Capacity) Reserve(m_Capacity * GROW_FACTOR + 1);
        if constexpr (std::is_trivial<T>::value)
            m_Data[m_Size] = value;
        else
            new (m_Data + m_Size) T(value);

        m_Size++;
    }

    template<typename T>
    void Vector<T>::Push_Back(T&& value) {
        if (m_Size == m_Capacity) Reserve(m_Capacity * GROW_FACTOR + 1);
        if constexpr (std::is_trivial<T>::value)
            m_Data[m_Size] = value;
        else
            new (m_Data + m_Size) T(ST_MOV(value));

        m_Size++;
    }

    template<typename T>
    void Vector<T>::Pop_Back() {
        ST_STATIC_ASSERT(m_Size > 0);  // Container is empty

        if constexpr (!std::is_trivial<T>::value) m_Data[m_Size - 1].~T();

        m_Size--;
    }

    template<typename T>
    void Vector<T>::Reserve(UInt32 count) {
        ST_STATIC_ASSERT(count > m_Capacity);

        if constexpr (std::is_trivial<T>::value) {
            m_Data = (T*)Realloc(m_Data, sizeof(T) * count);
            ST_STATIC_ASSERT(m_DATA != nullptr);

        } else {
            T* temp = (T*)Malloc(sizeof(T) * count);
            ST_STATIC_ASSERT(m_Data != nullptr);

            VectorUtils::copy_range(Begin(), End(), temp);
            VectorUtils::destruct_range(Begin(), End());

            Free(m_Data);

            m_Data = temp;
        }

        m_Capacity = count;
    }

    template<typename T>
    template<class... Args>
    void Sentinel::Vector<T>::Emplace_Back(Args&&... args) {
        // Use "Push_Back()" instead of "Emplace_Back()" for trivial types
        ST_STATIC_ASSERT(!std::is_trivial<T>::value);

        if (m_Size == m_Capacity) Reserve(m_Capacity * GROW_FACTOR + 1);

        new (m_Data + m_Size) T(ST_MOV(args)...);

        m_Size++;
    }
}  // namespace Sentinel
