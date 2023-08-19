#include "stpch.h"
#include "Sentinel/Common/Strings/WStringView.h"

namespace Sentinel {
    WStringView::WStringView(const WStringView& other) : m_Data(other.m_Data), m_Size(other.m_Size) {
    }

    WStringView::WStringView(WStringView&& other) noexcept {
        m_Data = other.m_Data;
        m_Size = other.m_Size;

        other.m_Data = nullptr;
        other.m_Size = 0;
    }

    WStringView::WStringView(WChar* string, UInt32 count) : m_Data(string), m_Size(count) {
    }

    WStringView::WStringView(WChar* string) : m_Data(string), m_Size(string != nullptr ? (UInt32)wcslen(string) : 0) {
    }

    WStringView::WStringView(const WString& string)
        : m_Data(const_cast<WChar*>(string.C_Str())), m_Size(string.Length()) {
    }

    WStringView& WStringView::operator=(const WStringView& other) {
        if (this != &other) {
            m_Data = other.m_Data;
            m_Size = other.m_Size;
        }

        return *this;
    }

    WStringView& WStringView::operator=(WStringView&& other) noexcept {
        if (this != &other) {
            m_Size = other.m_Size;
            m_Data = other.m_Data;

            other.m_Size = 0;
            other.m_Data = nullptr;
        }

        return *this;
    }

    inline Bool WStringView::Empty() const {
        return m_Size == 0;
    }

    inline CWChar* WStringView::C_Str() const {
        return m_Data;
    }
}  // namespace Sentinel
