#include "stpch.h"
#include "Sentinel/Common/Strings/StringView.h"

namespace Sentinel {
    StringView::StringView(const StringView& other) : m_Data(other.m_Data), m_Size(other.m_Size) {
    }

    StringView::StringView(StringView&& other) noexcept {
        m_Data = other.m_Data;
        m_Size = other.m_Size;

        other.m_Data = nullptr;
        other.m_Size = 0;
    }

    StringView::StringView(Char* string, UInt32 count) : m_Data(string), m_Size(count) {
    }

    StringView::StringView(Char* string) : m_Data(string), m_Size(string != nullptr ? (UInt32)strlen(string) : 0) {
    }

    StringView::StringView(const String& string) : m_Data(const_cast<Char*>(string.C_Str())), m_Size(string.Length()) {
    }

    StringView& StringView::operator=(const StringView& other) {
        if (this != &other) {
            m_Data = other.m_Data;
            m_Size = other.m_Size;
        }

        return *this;
    }

    StringView& StringView::operator=(StringView&& other) noexcept {
        if (this != &other) {
            m_Size = other.m_Size;
            m_Data = other.m_Data;

            other.m_Size = 0;
            other.m_Data = nullptr;
        }

        return *this;
    }

    inline Bool StringView::Empty() const {
        return m_Size == 0;
    }

    inline CChar* StringView::C_Str() const {
        return m_Data;
    }

    inline UInt32 StringView::Size() const {
        return m_Size;
    }
}  // namespace Sentinel