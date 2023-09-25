#pragma once

#include "Sentinel/Common/Strings/String.h"

namespace Sentinel {
    struct StringView {
    public:
        StringView() = default;
        StringView(const StringView& other);
        StringView(StringView&& other) noexcept;
        StringView(Char* string, UInt32 count);
        StringView(Char* string);
        StringView(const String& string);

        StringView& operator=(const StringView& other);
        StringView& operator=(StringView&& other) noexcept;

        inline Bool Empty() const;
        inline CChar* C_Str() const { return m_Data; }
        inline UInt32 Size() const;

    private:
        Char* m_Data = nullptr;
        UInt32 m_Size = 0;
    };
}  // namespace Sentinel