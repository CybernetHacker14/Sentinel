#include "stpch.h"
#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Output/FramebufferAPI.h"

#include "Platform/DirectX11/Graphics/Output/DX11FramebufferData.h"
#include "Platform/DirectX11/Graphics/Output/DX11FramebufferAPI.h"

#include <glm/glm.hpp>

namespace Sentinel {
    FramebufferData* FramebufferAPI::CreateFramebufferData(
        SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context, UInt32 width, UInt32 height) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                FramebufferData* framebuffer = memoryHandle->FramebufferAllocator.New<DX11FramebufferData>();
                framebuffer->Context = context;
                Invalidate(framebuffer, memoryHandle);
                return framebuffer;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel