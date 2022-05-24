#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/Texture2DAPI.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Materials/DX11Texture2DData.h"
#include "Platform/DirectX11/Graphics/Components/RenderResources/Materials/DX11Texture2DAPI.h"

namespace Sentinel {
    Texture2DData* Texture2DAPI::CreateTexture2DData(const Texture2DDataImportSettings& settings) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                Texture2DData* texObject =
                    Application::Get().EngineMemoryManager->Texture2DAllocator.New<DX11Texture2DData>();
                texObject->m_Settings = settings;
                DX11Texture2DAPI::Load(Texture2DAPI::Cast<DX11Texture2DData>(texObject));
                return texObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel