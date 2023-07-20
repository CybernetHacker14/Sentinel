#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    struct String {
    public:
        static constexpr UInt32 npos = static_cast<UInt32>(-1);

    public:
        String();
        String(CChar* data);
        String(const String& other);

        String(String&& other);

        ~String();

        String& operator=(const String& other);
        String& operator=(String&& other);

        Char& operator[](UInt8 index);
        CChar& operator[](UInt8 index) const;

    private:
        Char* Data;
        UInt32 Size;
    };

}  // namespace Sentinel
