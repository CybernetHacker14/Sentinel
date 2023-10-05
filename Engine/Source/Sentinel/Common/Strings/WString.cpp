#include "stpch.h"
#include "Sentinel/Common/Strings/WString.h"
#include "Sentinel/Common/Strings/MemFunctions.h"
#include "Sentinel/Common/Core/Malloc.h"
#include "Sentinel/Common/Core/Assert.h"

namespace Sentinel {
    WString::WString() {
        Set(L"", 0);
    }

    WString::WString(CWChar* data) {
        Set(data, static_cast<UInt32>(wcslen(data)));
    }

    WString::WString(const WString& other) {
        Set(other.C_Str(), other.Length());
    }

    WString::WString(WString&& other) noexcept {
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

    WString::~WString() {
        if (!m_SSO) {
            Free(m_Data.Data.heap.Data);
            m_Data.Data.heap.Data = nullptr;
        }
    }

    WString& WString::operator=(const WString& other) {
        if (this != &other) Set(other.C_Str(), other.Length());

        return *this;
    }

    WString& WString::operator=(WString&& other) noexcept {
        if (this != &other) {
            m_Data = other.m_Data;
            m_RemainingSize = other.m_RemainingSize;
            m_Empty = other.m_Empty;
            m_SSO = other.m_SSO;
            m_StringViewRefCount = other.m_StringViewRefCount;

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

    inline WChar& WString::operator[](UInt32 index) {
        return m_SSO ? m_Data.Data.sso.Data[index] : m_Data.Data.heap.Data[index];
    }

    inline CWChar& WString::operator[](UInt32 index) const {
        return m_SSO ? m_Data.Data.sso.Data[index] : m_Data.Data.heap.Data[index];
    }

    inline WChar* WString::Data() {
        return m_SSO ? &(m_Data.Data.sso.Data[0]) : m_Data.Data.heap.Data;
    }

    inline CWChar* WString::C_Str() const {
        return m_SSO ? &(m_Data.Data.sso.Data[0]) : m_Data.Data.heap.Data;
    }

    inline UInt32 WString::Length() const {
        return m_SSO ? m_Data.Data.sso.Size : m_Data.Data.heap.Size;
    }

    UInt64 WString::Hash() const {
        UInt64 hash = 2166136261UL;
        UInt32 size = m_SSO ? m_Data.Data.sso.Size : m_Data.Data.heap.Size;
        UChar* p = (UChar*)C_Str();

        for (UInt32 i = 0; i < size; ++i) hash = (hash ^ p[i]) * 16777619;

        return hash;
    }

    inline void WString::SetHeapCapacity(UInt32 capacity) {
        m_Data.Data.heap.Capacity = capacity;
    }

    inline Bool WString::Empty() const {
        return m_Empty;
    }

    WString& WString::Set(CWChar* data, UInt32 size, UInt32 start) {
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
            m_Data.Data.heap.Data = (WChar*)Malloc(newSize + 1);
            m_Data.Data.heap.Data[newSize] = '\0';

            MemFunctions::Memcpy(m_Data.Data.heap.Data, data + start, newSize);
        }

        m_SSO = newSize < SHORT_STRING_SIZE;
        m_Empty = newSize == 0;
        m_RemainingSize = m_SSO ? SHORT_STRING_SIZE - newSize : m_Data.Data.heap.Capacity - newSize;

        return *this;
    }

    UInt32 WString::GetSize() const {
        return m_SSO ? m_Data.Data.sso.Size : m_Data.Data.heap.Size;
    }
}  // namespace Sentinel
