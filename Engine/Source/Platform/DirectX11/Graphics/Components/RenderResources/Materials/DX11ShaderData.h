#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Materials/ShaderData.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

namespace Sentinel {
    struct DX11ShaderData final: public ShaderData {
    public:
        inline DX11ShaderData() { m_BinaryMap.reserve(4); }

    private:
        STL::unordered_map<ShaderType, ID3DBlob*> m_BinaryMap;

        ID3D11VertexShader* m_VertexShader = nullptr;
        ID3D11PixelShader* m_PixelShader = nullptr;
        ID3D11ComputeShader* m_ComputeShader = nullptr;

    private:
        friend class DX11ShaderAPI;
    };
}  // namespace Sentinel