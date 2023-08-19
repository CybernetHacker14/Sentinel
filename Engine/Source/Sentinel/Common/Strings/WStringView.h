#pragma once

#include "Sentinel/Common/Strings/WString.h"

namespace Sentinel {
    struct WStringView {
    public:
        WStringView() = default;
        WStringView(const WStringView& other);
        WStringView(WStringView&& other) noexcept;
        WStringView(WChar* string, UInt32 count);
        WStringView(WChar* string);
        WStringView(const WString& string);

        WStringView& operator=(const WStringView& other);
        WStringView& operator=(WStringView&& other) noexcept;

        inline Bool Empty() const;
        inline CWChar* C_Str() const;

    private:
        WChar* m_Data = nullptr;
        UInt32 m_Size = 0;
    };
}  // namespace Sentinel
