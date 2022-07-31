#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"

#include <glm/fwd.hpp>

namespace Sentinel {
    class GraphicsMemoryManager;
    struct SwapchainData;

    class RenderTexture2DAPI {
    public:
        static RenderTexture2DData* CreateRenderTexture2DData(
            SharedRef<GraphicsMemoryManager> memoryHandle,
            ContextData* context,
            const UInt32 width,
            const UInt32 height,
            const ColorFormat format,
            const Bool attachToSwapchain = false);

        inline static void Clear(RenderTexture2DData* dataObject, const glm::vec4& clearColor) {
            if (!m_ClearFunction) return;
            m_ClearFunction(dataObject, clearColor);
        }

        inline static void Clean(RenderTexture2DData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static void Resize(RenderTexture2DData* dataObject, UInt32 width, UInt32 height) {
            if (!m_ResizeFunction) return;
            m_ResizeFunction(dataObject, width, height);
        }

        inline static void Bind(RenderTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject, slot, shaderType);
        }

        inline static void Unbind(RenderTexture2DData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        inline static void* GetPixelData(RenderTexture2DData* dataObject) { return dataObject->m_PixelData; }

    public:
        template<typename T>
        inline static T* Cast(RenderTexture2DData* dataObject) {
            static_assert(
                STL::is_base_of<RenderTexture2DData, T>::value, "'T' should be derived from RenderTexture2DData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(RenderTexture2DData*, const glm::vec4&)> m_ClearFunction;
        inline static STL::delegate<void(RenderTexture2DData*)> m_CleanFunction;
        inline static STL::delegate<void(RenderTexture2DData*, UInt32, UInt32)> m_ResizeFunction;

        inline static STL::delegate<void(RenderTexture2DData*, UInt32, const ShaderType)> m_BindFunction;
        inline static STL::delegate<void(RenderTexture2DData*)> m_UnbindFunction;
    };
}  // namespace Sentinel