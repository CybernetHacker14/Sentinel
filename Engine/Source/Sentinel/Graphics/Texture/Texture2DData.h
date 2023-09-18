#pragma once

#include "Sentinel/Graphics/Material/ShaderData.h"

#ifdef ST_RENDERER_DX11
struct ID3D11Texture2D;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    enum class TextureWrapMode : UInt8 { REPEAT = 0, CLAMP = 1, MIRROR = 2, MIRROR_ONCE = 3 };
    struct ContextData;

    struct Texture2DDataImportSettings {
        CChar* TextureFilepath;

        TextureWrapMode WrapMode = TextureWrapMode::CLAMP;

        Bool ReadWriteEnabled = false;
        Bool GenerateMipMaps = false;
        Bool sRGB = false;
    };

    struct Texture2DData {
    public:
        ContextData* Context;

    private:
        Texture2DDataImportSettings m_Settings;
        void* m_TexturePixels = nullptr;
        UInt16 m_Width = 0, m_Height = 0;
        UInt8 m_BindSlot;
        ShaderType m_BindType = ShaderType::PIXEL;
        Bool m_Loaded = false, m_HDR = false;

    private:
        // Putting native resources here
#ifdef ST_RENDERER_DX11
        ID3D11Texture2D* m_Texture2D;
        ID3D11SamplerState* m_SamplerState;
        ID3D11ShaderResourceView* m_ResourceView;
#endif  // ST_RENDERER_DX11

        friend class Texture2DAPI;
    };
}  // namespace Sentinel