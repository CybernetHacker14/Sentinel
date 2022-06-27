#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"
#include "Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h"

#include "Sentinel/Graphics/Material/ShaderData.h"

#include "Platform/DirectX11/Graphics/Buffer/DX11VertexbufferLayoutData.h"

namespace Sentinel {
    VertexbufferLayoutData* VertexbufferLayoutAPI::CreateVertexbufferLayoutData(
        SharedRef<GraphicsMemoryManager> memoryHandle) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                VertexbufferLayoutData* layoutObject = memoryHandle->LayoutAllocator.New<DX11VertexbufferLayoutData>();
                return layoutObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel