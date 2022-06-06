#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/ConstantbufferAPI.h"

#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11ConstantbufferData.h"
#include "Platform/DirectX11/Graphics/Components/RenderResources/Buffers/DX11ConstantbufferAPI.h"

namespace Sentinel {
    ConstantbufferData* ConstantbufferAPI::CreateConstantbufferData(
        UInt32 size, UInt32 bindSlot, CBufferUsageType usageType) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                ConstantbufferData* bufferObject =
                    Application::Get().EngineMemoryManager->ConstantbufferAllocator.New<DX11ConstantbufferData>();
                DX11ConstantbufferAPI::CreateNative(
                    ConstantbufferAPI::Cast<DX11ConstantbufferData>(bufferObject), size, bindSlot, usageType);
                bufferObject->m_Size = size;
                bufferObject->m_UsageType = usageType;
                return bufferObject;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }
}  // namespace Sentinel