#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Texture/Texture2DData.h"

namespace Sentinel {
    class GraphicsMemoryManager;

    class Texture2DAPI {
    public:
        static Texture2DData* CreateTexture2DData(
            SharedRef<GraphicsMemoryManager> memoryHandle, const Texture2DDataImportSettings& settings);

        inline static void Bind(Texture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject, slot, shaderType);
        }

        inline static void Unbind(Texture2DData* dataObject, UInt32 slot, const ShaderType shaderType) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject, slot, shaderType);
        }

        inline static void Clean(Texture2DData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static Bool IsHDR(Texture2DData* dataObject) { return dataObject->m_HDR; }
        inline static Bool IsLoaded(Texture2DData* dataObject) { return dataObject->m_Loaded; }

        inline static UInt32 GetWidth(Texture2DData* dataObject) { return dataObject->m_Width; }
        inline static UInt32 GetHeight(Texture2DData* dataObject) { return dataObject->m_Height; }

    public:
        template<typename T>
        inline static T* Cast(Texture2DData* dataObject) {
            static_assert(STL::is_base_of<Texture2DData, T>::value, "'T' should be a derived from Texture2DData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(Texture2DData*, UInt32, const ShaderType)> m_BindFunction;
        inline static STL::delegate<void(Texture2DData*, UInt32, const ShaderType)> m_UnbindFunction;
        inline static STL::delegate<void(Texture2DData*)> m_CleanFunction;
    };
}  // namespace Sentinel