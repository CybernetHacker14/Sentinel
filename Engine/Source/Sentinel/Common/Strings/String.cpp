#include "stpch.h"
#include "Sentinel/Common/Strings/String.h"
#include "Sentinel/Common/Strings/MemFunctions.h"
#include "Sentinel/Common/Core/Malloc.h"
#include "Sentinel/Common/Core/Assert.h"

namespace Sentinel {
    String::String() {
        Set("", 0);
    }

    String::String(CChar* data) {
        Set(data, static_cast<UInt32>(strlen(data)));
    }

    String::String(const String& other) {
        Set(other.C_Str(), other.Length());
    }

    String::String(String&& other) noexcept {
        // Resorting to default copy operation
        // For SSO, the entire char stack will be copied
        // For heap, the pointer to the char stack will be copied,
        // hence we aren't freeing it in the if(!other.m_SSO) block
        m_Data = other.m_Data;
        m_RemainingSize = other.m_RemainingSize;
        m_Empty = other.m_Empty;
        m_SSO = other.m_SSO;
        m_StringViewRefCount = other.m_StringViewRefCount;

        if (!other.m_SSO) {
            other.m_Data.Data.heap.Data = nullptr;
            other.m_Data.Data.heap.Size = 0;
        }

        other.m_Data.Data.sso.Size = 0;  // defaulting to SSO
        other.m_RemainingSize = 0;
        other.m_SSO = 0;
    }

    String::~String() {
        if (!m_SSO) {
            Free(m_Data.Data.heap.Data);
            m_Data.Data.heap.Data = nullptr;
        }
    }

    String& String::operator=(const String& other) {
        if (this != &other) Set(other.C_Str(), other.Length());

        return *this;
    }

    String& String::operator=(String&& other) noexcept {
        if (this == nullptr) Set("", 0);

        if (this != &other) {
            m_RemainingSize = other.m_RemainingSize;
            m_Empty = other.m_Empty;
            m_SSO = other.m_SSO;
            m_StringViewRefCount = other.m_StringViewRefCount;
            m_Data = other.m_Data;

            if (!other.m_SSO) {
                other.m_Data.Data.heap.Data = nullptr;
                other.m_Data.Data.heap.Size = 0;
            }

            other.m_Data.Data.sso.Size = 0;
            other.m_RemainingSize = 0;
            other.m_SSO = 0;
        }

        return *this;
    }

    inline Char& String::operator[](UInt32 index) {
        return m_SSO ? m_Data.Data.sso.Data[index] : m_Data.Data.heap.Data[index];
    }

    inline CChar& String::operator[](UInt32 index) const {
        return m_SSO ? m_Data.Data.sso.Data[index] : m_Data.Data.heap.Data[index];
    }

    inline Char* String::Data() {
        return m_SSO ? &(m_Data.Data.sso.Data[0]) : m_Data.Data.heap.Data;
    }

    inline CChar* String::C_Str() const {
        return m_SSO ? &(m_Data.Data.sso.Data[0]) : m_Data.Data.heap.Data;
    }

    inline UInt32 String::Length() const {
        return m_SSO ? m_Data.Data.sso.Size : m_Data.Data.heap.Size;
    }

    UInt64 String::Hash() const {
        UInt64 hash = 2166136261UL;
        UInt32 size = m_SSO ? m_Data.Data.sso.Size : m_Data.Data.heap.Size;
        UChar* p = (UChar*)C_Str();

        for (UInt32 i = 0; i < size; ++i) hash = (hash ^ p[i]) * 16777619;

        return hash;
    }

    inline void String::SetHeapCapacity(UInt32 capacity) {
        m_Data.Data.heap.Capacity = capacity;
    }

    inline Bool String::Empty() const {
        return m_Empty;
    }

    String& String::Append(CChar letter) {
        UInt32 size = Length() + 1;
        Char* data = (Char*)Malloc(size + 1);
        data[Length()] = letter;
        data[Length() + 1] = '\0';

        if (Data()) MemFunctions::Memcpy(data, Data(), Length());

        return Set(data, size);
    }

    String& String::Append(const String& other) {
        UInt32 size = Length() + other.Length();
        Char* data = (Char*)Malloc(size + 1);
        data[size] = '\0';

        if (Data()) MemFunctions::Memcpy(data, Data(), Length());

        MemFunctions::Memcpy((Char*)(data + Length()), other.C_Str(), other.Length());

        return Set(data, size);
    }

    // Somewhat inspired by a combination of eastl::string and HighLo-Engine String class
    String& String::Set(CChar* data, UInt32 size, UInt32 start) {
        ST_BREAKPOINT_ASSERT(data || start <= size, "Bad parameters")

        UInt32 newSize = size - start;

        if (!m_SSO) {
            Free(m_Data.Data.heap.Data);
            m_Data.Data.heap.Data = nullptr;
            m_Data.Data.heap.Size = 0;
        }

        if (newSize < SHORT_STRING_SIZE) {
            m_Data.Data.sso.Size = newSize;
            m_Data.Data.sso.Data[newSize] = '\0';

            MemFunctions::Memcpy(m_Data.Data.sso.Data, data + start, newSize);

        } else {
            m_Data.Data.heap.Size = newSize;
            m_Data.Data.heap.Data = (Char*)Malloc(newSize + 1);
            m_Data.Data.heap.Data[newSize] = '\0';

            MemFunctions::Memcpy(m_Data.Data.heap.Data, data + start, newSize);
        }

        m_SSO = newSize < SHORT_STRING_SIZE;
        m_Empty = newSize == 0;
        m_RemainingSize = m_SSO ? SHORT_STRING_SIZE - newSize : m_Data.Data.heap.Capacity - newSize;

        return *this;
    }

    UInt32 String::GetSize() const {
        return m_SSO ? m_Data.Data.sso.Size : m_Data.Data.heap.Size;
    }

}  // namespace Sentinel