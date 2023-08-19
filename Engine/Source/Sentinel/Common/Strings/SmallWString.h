#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/CompilerDetection.h"

#ifdef ST_COMPILER_CLANG
    #define SHORT_STRING_SIZE 22
#else
    #define SHORT_STRING_SIZE 15
#endif  // ST_COMPILER_CLANG

namespace Sentinel {
    struct SmallWString {
    public:
        static constexpr UInt32 npos = static_cast<UInt32>(-1);

    public:
        SmallWString();
        SmallWString(CWChar* data);
        SmallWString(const SmallWString& other);

        ~SmallWString();

        SmallWString& operator=(const SmallWString& other);
        WChar& operator[](UInt8 index);
        CWChar& operator[](UInt8 index) const;

        inline CWChar* C_Str() const { return &m_Data[0]; }

    private:
        WChar m_Data[SHORT_STRING_SIZE] = {0};
        UInt8 m_Size = 0;
    };
}  // namespace Sentinel
