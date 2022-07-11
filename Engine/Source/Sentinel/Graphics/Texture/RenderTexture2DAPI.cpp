#include "stpch.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"

#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DData.h"
#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DAPI.h"

#include <glm/glm.hpp>

namespace Sentinel {
    RenderTexture2DData* RenderTexture2DAPI::CreateRenderTexture2DData(
        SharedRef<GraphicsMemoryManager> memoryHandle,
        ContextData* context,
        UInt32 width,
        UInt32 height,
        ColorFormat colorFormat,
        DepthFormat depthFormat) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                RenderTexture2DData* texObject = memoryHandle->RenderTexture2DAllocator.New<DX11RenderTexture2DData>();
                texObject->Context = context;
                texObject->m_ColorFormat = colorFormat;
                texObject->m_DepthFormat = depthFormat;
                texObject->m_Width = width;
                texObject->m_Height = height;
                texObject->m_BindColorType = ShaderType::NONE;
                texObject->m_BindDepthType = ShaderType::NONE;
                DX11RenderTexture2DAPI::Create(RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(texObject));
                return texObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel