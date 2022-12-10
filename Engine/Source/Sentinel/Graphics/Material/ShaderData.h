#pragma once

#include "Sentinel/Common/Common.h"

#if ST_RENDERER_DX11
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    enum class ShaderType : UInt8 { NONE = 0, VERTEX = 1, PIXEL = 2, COMPUTE = 3 };
    struct ContextData;

    struct ShaderData {
    public:
        inline ShaderData() {
            m_ShaderSources.reserve(4);
            m_BinaryMap.reserve(4);
        }

    public:
        ContextData* Context;

    private:
        STL::string m_ShaderName;
        STL::string m_Filepath;
        STL::unordered_map<ShaderType, STL::string> m_ShaderSources;

    private:
#if ST_RENDERER_DX11
        STL::unordered_map<ShaderType, ID3DBlob*> m_BinaryMap;

        ID3D11VertexShader* m_NativeVS = nullptr;
        ID3D11PixelShader* m_NativePS = nullptr;
        ID3D11ComputeShader* m_NativeCS = nullptr;
#endif  // ST_RENDERER_DX11

        friend class ShaderAPI;
    };
}  // namespace Sentinel