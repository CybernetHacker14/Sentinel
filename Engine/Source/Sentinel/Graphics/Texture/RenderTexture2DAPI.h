#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"

#include <glm/fwd.hpp>

namespace Sentinel {
    class GraphicsMemoryManager;

    class RenderTexture2DAPI {
    public:
        static RenderTexture2DData* CreateRenderTexture2DData(
            SharedRef<GraphicsMemoryManager> memoryHandle,
            ContextData* context,
            UInt32 width,
            UInt32 height,
            ColorFormat colorFormat,
            DepthFormat depthFormat);

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

        inline static void BindColor(RenderTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
            if (!m_BindColorFunction) return;
            m_BindColorFunction(dataObject, slot, shaderType);
        }

        inline static void BindDepth(RenderTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
            if (!m_BindDepthFunction) return;
            m_BindDepthFunction(dataObject, slot, shaderType);
        }

        inline static void Unbind(RenderTexture2DData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        inline static UInt32 GetWidth(RenderTexture2DData* dataObject) { return dataObject->m_Width; }
        inline static UInt32 GetHeight(RenderTexture2DData* dataObject) { return dataObject->m_Height; }

        inline static UInt32 GetBindColorSlot(RenderTexture2DData* dataObject) { return dataObject->m_BindColorSlot; }
        inline static UInt32 GetBindDepthSlot(RenderTexture2DData* dataObject) { return dataObject->m_BindDepthSlot; }

        inline static const ShaderType GetBindColorType(RenderTexture2DData* dataObject) {
            return dataObject->m_BindColorType;
        }

        inline static const ShaderType GetBindDepthType(RenderTexture2DData* dataObject) {
            return dataObject->m_BindDepthType;
        }

        inline static void* GetPixelData(RenderTexture2DData* dataObject) { return dataObject->m_PixelData; }

    private:
        inline static void SetWidth(RenderTexture2DData* dataObject, UInt32 width) { dataObject->m_Width = width; }
        inline static void SetHeight(RenderTexture2DData* dataObject, UInt32 height) { dataObject->m_Height = height; }

        inline static void SetBindColorSlot(RenderTexture2DData* dataObject, UInt32 slot) {
            dataObject->m_BindColorSlot = slot;
        }

        inline static void SetBindDepthSlot(RenderTexture2DData* dataObject, UInt32 slot) {
            dataObject->m_BindDepthSlot = slot;
        }

        inline static void SetBindColorType(RenderTexture2DData* dataObject, ShaderType shaderType) {
            dataObject->m_BindColorType = shaderType;
        }

        inline static void SetBindDepthType(RenderTexture2DData* dataObject, ShaderType shaderType) {
            dataObject->m_BindDepthType = shaderType;
        }

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

        inline static STL::delegate<void(RenderTexture2DData*, UInt32, const ShaderType)> m_BindColorFunction;
        inline static STL::delegate<void(RenderTexture2DData*, UInt32, const ShaderType)> m_BindDepthFunction;
        inline static STL::delegate<void(RenderTexture2DData*)> m_UnbindFunction;

    private:
        friend class DX11RenderTexture2DAPI;
    };
}  // namespace Sentinel