#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11shadertracing.h>
#include <dxgi.h>
#include <wrl.h>

namespace Sentinel {

	inline Bool DisplayMessage(HRESULT hresult, STL::string_view statement, STL::string_view file, UInt32 line) {
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
            ST_ENGINE_ERROR("[{0}({1})] '{2}' failed with error: {3}", file.data(), line, statement.data(), buffer);
            return false;
        }
        return true;
	}

#define CHECK(x)\
	{\
		HRESULT r = x;\
		DisplayMessage((r), #x, __FILE__, __LINE__ );\
	}
}