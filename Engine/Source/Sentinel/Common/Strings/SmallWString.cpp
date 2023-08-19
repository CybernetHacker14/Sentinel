#include "stpch.h"
#include "Sentinel/Common/Strings/SmallWString.h"

#include "Sentinel/Common/Core/Assert.h"
#include "Sentinel/Common/Strings/MemFunctions.h"

namespace Sentinel {
    SmallWString::SmallWString() {
        m_Size = 0;
        m_Data[0] = L'\0';
    }

    SmallWString::SmallWString(CWChar* data) {
        if (!data) return;

        m_Size = (UInt8)wcslen(data);
        MemFunctions::Memcpy(m_Data, data, m_Size);
        m_Data[m_Size] = L'\0';
    }

    SmallWString::SmallWString(const SmallWString& other) {
        m_Size = other.m_Size;
        MemFunctions::Memcpy(m_Data, other.m_Data, m_Size);
        m_Data[m_Size] = L'\0';
    }

    SmallWString::~SmallWString() {
        MemFunctions::Memset(m_Data, 0, m_Size);
    }

    SmallWString& SmallWString::operator=(const SmallWString& other) {
        m_Size = other.m_Size;
        MemFunctions::Memcpy(m_Data, other.m_Data, m_Size);
        m_Data[m_Size] = L'\0';
        return *this;
    }

    WChar& SmallWString::operator[](UInt8 index) {
        ST_BREAKPOINT_ASSERT(index <= m_Size, "Index overflow") return m_Data[index];
    }

    CWChar& SmallWString::operator[](UInt8 index) const {
        ST_BREAKPOINT_ASSERT(index <= m_Size, "Index overflow") return m_Data[index];
    }
}  // namespace Sentinel
