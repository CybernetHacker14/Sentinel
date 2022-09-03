#include "stpch.h"
#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"

#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DAPI.h"
#include "Platform/DirectX11/Graphics/Device/DX11SwapchainAPI.h"

#include <glm/glm.hpp>

namespace Sentinel {
    RenderTexture2DData* RenderTexture2DAPI::CreateRenderTexture2DData(
        SharedRef<GraphicsMemoryManager> memoryHandle,
        ContextData* context,
        const UInt32 width,
        const UInt32 height,
        const ColorFormat format) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                RenderTexture2DData* texObject = memoryHandle->RenderTexture2DAllocator.New<DX11RenderTexture2DData>();
                texObject->Context = context;
                texObject->MemoryHandle = memoryHandle;
                texObject->m_Format = format;
                texObject->m_Width = width;
                texObject->m_Height = height;
                texObject->m_BindType = ShaderType::NONE;
                texObject->m_SwapchainTarget = false;
                DX11RenderTexture2DAPI::Create(RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(texObject));
                return texObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }

    RenderTexture2DData* RenderTexture2DAPI::CreateRenderTexture2DData(
        SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context, SwapchainData* swapchain) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                RenderTexture2DData* texObject = memoryHandle->RenderTexture2DAllocator.New<DX11RenderTexture2DData>();
                texObject->Context = context;
                texObject->MemoryHandle = memoryHandle;
                texObject->m_Format = ColorFormat::RGBA32F;
                texObject->m_BindType = ShaderType::NONE;
                texObject->m_SwapchainTarget = true;
                DX11RenderTexture2DAPI::Create(
                    RenderTexture2DAPI::Cast<DX11RenderTexture2DData>(texObject),
                    SwapchainAPI::Cast<DX11SwapchainData>(swapchain));
                return texObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel