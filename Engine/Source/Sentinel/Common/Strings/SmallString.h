#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/CompilerDetection.h"

#ifdef ST_COMPILER_CLANG
    #define SHORT_STRING_SIZE 22
#else
    #define SHORT_STRING_SIZE 15
#endif  // ST_COMPILER_CLANG

namespace Sentinel {
    struct SmallString {
    public:
        static constexpr UInt32 npos = static_cast<UInt32>(-1);

    public:
        SmallString();
        SmallString(CChar* data);
        SmallString(const SmallString& other);

        ~SmallString();

        SmallString& operator=(const SmallString& other);
        Char& operator[](UInt8 index);
        CChar& operator[](UInt8 index) const;

        inline CChar* C_Str() const { return &m_Data[0]; }

    private:
        Char m_Data[SHORT_STRING_SIZE] = {0};
        UInt8 m_Size = 0;
    };
}  // namespace Sentinel
