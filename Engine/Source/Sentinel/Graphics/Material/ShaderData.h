#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

#ifdef ST_RENDERER_DX11
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    enum class ShaderType : UInt8 { VERTEX = 0, PIXEL = 1, COMPUTE = 2 };
    struct ContextData;

    struct ShaderData {
    public:
        ContextData* Context;

    private:
        CChar* m_ShaderName;
        CChar* m_Filepath;
        CChar* m_Sources[3];

#ifdef ST_RENDERER_DX11
        ID3DBlob* m_Binaries[3];

        ID3D11VertexShader* m_NativeVS = nullptr;
        ID3D11PixelShader* m_NativePS = nullptr;
        ID3D11ComputeShader* m_NativeCS = nullptr;
#endif  // ST_RENDERER_DX11

    private:
        friend class ShaderAPI;
    };
}  // namespace Sentinel