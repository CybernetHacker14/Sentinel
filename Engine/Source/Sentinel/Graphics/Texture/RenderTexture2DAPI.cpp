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
        const UInt32 width,
        const UInt32 height,
        const ColorFormat format,
        const Bool attachToSwapchain) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                RenderTexture2DData* texObject = memoryHandle->RenderTexture2DAllocator.New<DX11RenderTexture2DData>();
                texObject->Context = context;
                texObject->MemoryHandle = memoryHandle;
                texObject->m_Format = format;
                texObject->m_Width = width;
                texObject->m_Height = height;
                texObject->m_BindType = ShaderType::NONE;
                texObject->m_SwapchainTarget = attachToSwapchain;
                DX11RenderTexture2DAPI::Create(RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(texObject));
                return texObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel