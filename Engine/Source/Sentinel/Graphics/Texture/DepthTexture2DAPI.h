#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DData.h"

#include <glm/fwd.hpp>

namespace Sentinel {
    class GraphicsMemoryManager;
    struct SwapchainData;

    class DepthTexture2DAPI {
    public:
        static DepthTexture2DData* CreateDepthTexture2DData(
            SharedRef<GraphicsMemoryManager> memoryHandle,
            ContextData* context,
            const UInt32 width,
            const UInt32 height,
            const DepthFormat format,
            const Bool attachToSwapchain = false);

        inline static void Clear(DepthTexture2DData* dataObject) {
            if (!m_ClearFunction) return;
            m_ClearFunction(dataObject);
        }

        inline static void Clean(DepthTexture2DData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static void Bind(DepthTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject, slot, shaderType);
        }

        inline static void Unbind(DepthTexture2DData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        inline static void Resize(DepthTexture2DData* dataObject, UInt16 width, UInt16 height) {
            if (!m_ResizeFunction) return;
            m_ResizeFunction(dataObject, width, height);
        }

        inline static void SetSwapchainTarget(DepthTexture2DData* dataObject, Bool value) {
            dataObject->m_SwapchainTarget = value;
        }

        inline static void* GetPixelData(DepthTexture2DData* dataObject) { return dataObject->m_PixelData; }

    public:
        template<typename T>
        inline static T* Cast(DepthTexture2DData* dataObject) {
            static_assert(
                STL::is_base_of<DepthTexture2DData, T>::value, "'T' should be derived from DepthTexture2DData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(DepthTexture2DData*)> m_ClearFunction;
        inline static STL::delegate<void(DepthTexture2DData*)> m_CleanFunction;
        inline static STL::delegate<void(DepthTexture2DData*, UInt32, UInt32)> m_ResizeFunction;

        inline static STL::delegate<void(DepthTexture2DData*, UInt32, const ShaderType)> m_BindFunction;
        inline static STL::delegate<void(DepthTexture2DData*)> m_UnbindFunction;
    };
}  // namespace Sentinel