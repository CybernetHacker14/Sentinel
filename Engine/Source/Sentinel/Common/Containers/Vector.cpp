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
    Vector<T>::Vector(const Vector& other) {
    }

    template<typename T>
    Vector<T>::Vector(Vector&& other) noexcept {
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector& other) {
        return *this;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
        return *this;
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
}  // namespace Sentinel