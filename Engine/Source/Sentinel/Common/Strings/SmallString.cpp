#include "stpch.h"
#include "Sentinel/Common/Strings/SmallString.h"

#include "Sentinel/Common/Core/Assert.h"
#include "Sentinel/Common/Strings/MemFunctions.h"

namespace Sentinel {
    SmallString::SmallString() {
        m_Size = 0;
        m_Data[0] = '\0';
    }

    SmallString::SmallString(CChar* data) {
        if (!data) return;

        m_Size = (UInt8)strlen(data);  // I hope that the compiler replaces this with some assembly magic
        MemFunctions::Memcpy(m_Data, data, m_Size);
        m_Data[m_Size] = '\0';
    }

    SmallString::SmallString(const SmallString& other) {
        m_Size = other.m_Size;
        MemFunctions::Memcpy(m_Data, other.m_Data, m_Size);
        m_Data[m_Size] = '\0';
    }

    SmallString::~SmallString() {
        MemFunctions::Memset(m_Data, 0, m_Size);
    }

    SmallString& SmallString::operator=(const SmallString& other) {
        m_Size = other.m_Size;
        MemFunctions::Memcpy(m_Data, other.m_Data, m_Size);
        m_Data[m_Size] = '\0';
        return *this;
    }

    Char& SmallString::operator[](UInt8 index) {
        ST_BREAKPOINT_ASSERT(index <= m_Size, "Index overflow") return m_Data[index];
    }

    CChar& SmallString::operator[](UInt8 index) const {
        ST_BREAKPOINT_ASSERT(index <= m_Size, "Index overflow") return m_Data[index];
    }
}  // namespace Sentinel
