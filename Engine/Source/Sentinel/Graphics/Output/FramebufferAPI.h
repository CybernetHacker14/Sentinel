#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Output/FramebufferData.h"

#include <glm/fwd.hpp>

namespace Sentinel {
    class GraphicsMemoryManager;

    class FramebufferAPI {
    public:
        static FramebufferData* CreateFramebufferData(
            SharedRef<GraphicsMemoryManager> memoryHandle, ContextData* context, UInt32 width, UInt32 height);

        inline static void Invalidate(FramebufferData* dataObject, SharedRef<GraphicsMemoryManager> memoryHandle) {
            if (!m_InvalidateFunction) return;
            m_InvalidateFunction(dataObject, memoryHandle);
        }

        inline static void Bind(FramebufferData* dataObject) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject);
        }

        inline static void Unbind(FramebufferData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        inline static void Clear(FramebufferData* dataObject, const glm::vec4& clearColor) {
            if (!m_ClearFunction) return;
            m_ClearFunction(dataObject, clearColor);
        }

        inline static void Resize(FramebufferData* dataObject, const UInt32 width, const UInt32 height) {
            if (!m_ResizeFunction) return;
            m_ResizeFunction(dataObject, width, height);
        }

        inline static void Clean(FramebufferData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static void SetAttachments(
            FramebufferData* dataObject,
            const STL::initializer_list<ColorFormat>& attachments,
            const DepthFormat depthFormat = DepthFormat::NONE) {
            dataObject->m_ColorFormats = attachments;
            dataObject->m_DepthFormat = depthFormat;
        }

        inline static const ColorFormat GetColorFormat(FramebufferData* dataObject, UInt16 index) {
            return dataObject->m_ColorFormats[index];
        }

        inline static const DepthFormat GetDepthFormat(FramebufferData* dataObject) {
            return dataObject->m_DepthFormat;
        }

        inline static RenderTexture2DData* GetRenderTexture(FramebufferData* dataObject, UInt16 index) {
            return dataObject->m_RTAttachments[index];
        }

        inline static void SetRenderTexture(
            FramebufferData* dataObject, UInt16 index, RenderTexture2DData* renderTexture) {
            dataObject->m_RTAttachments[index] = renderTexture;
        }

        inline static const UInt32 GetWidth(FramebufferData* dataObject) { return dataObject->m_Width; }
        inline static const UInt32 GetHeight(FramebufferData* dataObject) { return dataObject->m_Height; }

        inline static void SetWidth(FramebufferData* dataObject, const UInt32 width) { dataObject->m_Width = width; }
        inline static void SetHeight(FramebufferData* dataObject, const UInt32 height) {
            dataObject->m_Height = height;
        }

    public:
        template<typename T>
        inline static T* Cast(FramebufferData* dataObject) {
            static_assert(STL::is_base_of<FramebufferData, T>::value, "'T' should be derived from FramebufferData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(FramebufferData*, SharedRef<GraphicsMemoryManager>)> m_InvalidateFunction;
        inline static STL::delegate<void(FramebufferData*)> m_BindFunction;
        inline static STL::delegate<void(FramebufferData*)> m_UnbindFunction;
        inline static STL::delegate<void(FramebufferData*, const glm::vec4&)> m_ClearFunction;
        inline static STL::delegate<void(FramebufferData*, const UInt32, const UInt32)> m_ResizeFunction;
        inline static STL::delegate<void(FramebufferData*)> m_CleanFunction;
    };
}  // namespace Sentinel