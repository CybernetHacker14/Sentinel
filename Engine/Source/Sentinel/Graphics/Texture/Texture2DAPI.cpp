#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Texture/Texture2DAPI.h"

#include "Platform/DirectX11/Graphics/Texture/DX11Texture2DData.h"
#include "Platform/DirectX11/Graphics/Texture/DX11Texture2DAPI.h"

namespace Sentinel {
    Texture2DData* Texture2DAPI::CreateTexture2DData(
        SharedRef<GraphicsMemoryManager> memoryHandle,
        ContextData* context,
        const Texture2DDataImportSettings& settings) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                Texture2DData* texObject = memoryHandle->Texture2DAllocator.New<DX11Texture2DData>();
                texObject->Context = context;
                texObject->m_Settings = settings;
                texObject->m_BindType = ShaderType::NONE;
                DX11Texture2DAPI::Load(Texture2DAPI::Cast<DX11Texture2DData>(texObject));
                return texObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }

    Texture2DData* Texture2DAPI::CreateTexture2DData(
        PoolAllocator<Texture2DData> allocator, ContextData* context, const Texture2DDataImportSettings& settings) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                Texture2DData* texObject = allocator.New<DX11Texture2DData>();
                texObject->Context = context;
                texObject->m_Settings = settings;
                texObject->m_BindType = ShaderType::NONE;
                DX11Texture2DAPI::Load(Texture2DAPI::Cast<DX11Texture2DData>(texObject));
                return texObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }

    Texture2DData* Texture2DAPI::CreateTexture2DData(
        PoolAllocator<Texture2DData> allocator,
        ContextData* context,
        const Texture2DDataImportSettings& settings,
        UInt8* pixelData,
        UInt32 width,
        UInt32 height,
        UInt8 channels) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                Texture2DData* texObject = allocator.New<DX11Texture2DData>();
                texObject->m_TexturePixels = pixelData;
                texObject->Context = context;
                texObject->m_Settings = settings;
                texObject->m_BindType = ShaderType::NONE;
                DX11Texture2DAPI::Load(Texture2DAPI::Cast<DX11Texture2DData>(texObject), width, height, channels);
                return texObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel