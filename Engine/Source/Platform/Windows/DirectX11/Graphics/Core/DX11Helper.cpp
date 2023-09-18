#include "stpch.h"

#ifdef ST_RENDERER_DX11
    #include "Platform/Windows/DirectX11/Graphics/Core/DX11Helper.h"
    #include "Sentinel/Common/Core/Assert.h"
    #include "Sentinel/Logging/Log.h"

namespace Sentinel {
    Bool DisplayMessage(HRESULT hresult, StringView statement, StringView file, UInt32 line) {
        if (FAILED(hresult)) {
            LPSTR buffer;
            FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                hresult,
                0,
                reinterpret_cast<LPSTR>(&buffer),
                0,
                nullptr);
            ST_TERMINAL_ERROR("[%s(%u)] '%s' failed with error: %s", file.C_Str(), line, statement.C_Str(), buffer);
            return false;
        }
        return true;
    }
}  // namespace Sentinel

#endif  // ST_RENDERER_DX11
