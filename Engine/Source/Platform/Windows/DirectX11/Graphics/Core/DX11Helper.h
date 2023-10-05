#pragma once

#include "Sentinel/Common/Strings/StringView.h"

namespace Sentinel {
    inline Bool DisplayMessage(HRESULT hresult, StringView statement, StringView file, UInt32 line);

#define CHECK(x)                                     \
    {                                                \
        HRESULT r = x;                               \
        DisplayMessage((r), #x, __FILE__, __LINE__); \
    }
}  // namespace Sentinel