#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/IndexbufferAPI.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11IndexbufferData.h"
#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11IndexbufferAPI.h"

namespace Sentinel {
    IndexbufferData* IndexbufferAPI::CreateIndexbufferData(void* indices, UInt32 count) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                IndexbufferData* bufferObject =
                    Application::Get().EngineMemoryManager->IndexbufferAllocator.New<DX11IndexbufferData>();
                DX11IndexbufferAPI::CreateNative(
                    IndexbufferAPI::Cast<DX11IndexbufferData>(bufferObject), indices, count);
                bufferObject->m_Count = count;
                return bufferObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel