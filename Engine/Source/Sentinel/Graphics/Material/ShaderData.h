#pragma once

#include "Sentinel/Common/Common.h"

#ifdef ST_RENDERER_DX11
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    enum class ShaderType : UInt8 { NONE = 0, VERTEX = 1, PIXEL = 2, COMPUTE = 3 };
    struct ContextData;

    struct ShaderSource {
        STL::string source;
        ShaderType type;
    };

#ifdef ST_RENDERER_DX11
    struct ShaderBinary {
        ID3DBlob* binary = NULL;
        ShaderType type;
    };
#endif  // ST_RENDERER_DX11

    struct ShaderData {
    public:
        ContextData* Context;

    private:
        STL::string m_ShaderName;
        STL::string m_Filepath;
        ShaderSource m_Sources[4];

#ifdef ST_RENDERER_DX11
        ShaderBinary m_Binaries[4];

        ID3D11VertexShader* m_NativeVS = nullptr;
        ID3D11PixelShader* m_NativePS = nullptr;
        ID3D11ComputeShader* m_NativeCS = nullptr;
#endif  // ST_RENDERER_DX11

    private:
        friend class ShaderAPI;
    };
}  // namespace Sentinel