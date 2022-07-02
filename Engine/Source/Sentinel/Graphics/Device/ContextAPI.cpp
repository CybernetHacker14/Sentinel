#include "stpch.h"

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Device/ContextAPI.h"

#include "Sentinel/Graphics/Common/GraphicsMemoryManager.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextData.h"
#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

namespace Sentinel {
    ContextData* ContextAPI::CreateImmediateContext(SharedRef<GraphicsMemoryManager> memoryHandle) {
        switch (Backend::GetAPI()) {
            case Backend::API::DirectX11: {
                ContextData* context = memoryHandle->ContextAllocator.New<DX11ContextData>();
                context->m_ContextType = ContextType::IMMEDIATE;
                DX11ContextAPI::Create(ContextAPI::Cast<DX11ContextData>(context));
                return context;
            }
            case Backend::API::None: ST_ENGINE_ASSERT(false, "API::None currently not supported"); return nullptr;
        }
        ST_ENGINE_ASSERT(false, "Unknown Backend API");
        return nullptr;
    }

    ContextData* ContextAPI::CreateDeferredContext(SharedRef<GraphicsMemoryManager> memoryHandle) { return nullptr; }
}  // namespace Sentinel