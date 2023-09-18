#pragma once

#include "Sentinel/Memory/FixedSlabAllocator.h"
#include "Sentinel/Graphics/Texture/Texture2DData.h"

namespace Sentinel {
    class Texture2DAPI {
    public:
        static Texture2DData* CreateTexture2DData(
            FixedSlabAllocator<Texture2DData>& allocator,
            ContextData* context,
            const Texture2DDataImportSettings& settings);

        static Texture2DData* CreateTexture2DData(
            FixedSlabAllocator<Texture2DData>& allocator,
            ContextData* context,
            const Texture2DDataImportSettings& settings,
            UInt8* pixelData,
            const UInt32 width,
            const UInt32 height,
            const UInt8 channels);

        static void Bind(Texture2DData* dataObject, UInt32 slot, const ShaderType shaderType);

        static void Unbind(Texture2DData* dataObject);

        static void Clean(Texture2DData* dataObject);

        static void* GetResource(Texture2DData* dataObject);

        inline static Bool IsHDR(Texture2DData* dataObject) { return dataObject->m_HDR; }

        inline static Bool IsLoaded(Texture2DData* dataObject) { return dataObject->m_Loaded; }

        inline static UInt16 GetWidth(Texture2DData* dataObject) { return dataObject->m_Width; }

        inline static UInt16 GetHeight(Texture2DData* dataObject) { return dataObject->m_Height; }

        inline static UInt8 GetBindSlot(Texture2DData* dataObject) { return dataObject->m_BindSlot; }

        inline static const ShaderType GetBindType(Texture2DData* dataObject) { return dataObject->m_BindType; }

        inline static void* GetPixelData(Texture2DData* dataObject) { return dataObject->m_TexturePixels; }

#ifdef ST_RENDERER_DX11
        inline static ID3D11Texture2D* GetNativeTexture(Texture2DData* dataObject) { return dataObject->m_Texture2D; }

        inline static ID3D11SamplerState* GetNativeSampler(Texture2DData* dataObject) {
            return dataObject->m_SamplerState;
        }

        inline static ID3D11ShaderResourceView* GetNativeResource(Texture2DData* dataObject) {
            return dataObject->m_ResourceView;
        }
#endif  // ST_RENDERER_DX11

    private:
        static void Load(Texture2DData* data);
        static void Load(Texture2DData* data, UInt16 width, UInt16 height, UInt8 channels);
    };
}  // namespace Sentinel