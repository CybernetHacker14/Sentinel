#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Material/ShaderData.h"

namespace Sentinel {
    class GraphicsMemoryManager;

    class ShaderAPI {
    public:
        static ShaderData* CreateShaderData(
            SharedRef<GraphicsMemoryManager> memoryHandle,
            ContextData* context,
            const STL::string& filepath,
            const STL::string& name);

        inline static void Bind(ShaderData* dataObject) {
            if (!m_BindFunction) return;
            m_BindFunction(dataObject);
        }

        inline static void Reload(ShaderData* dataObject) {
            if (!m_ReloadFunction) return;
            m_ReloadFunction(dataObject);
        }

        inline static void Clean(ShaderData* dataObject) {
            if (!m_CleanFunction) return;
            m_CleanFunction(dataObject);
        }

        inline static void Unbind(ShaderData* dataObject) {
            if (!m_UnbindFunction) return;
            m_UnbindFunction(dataObject);
        }

        inline static const STL::string& GetShaderSource(
            ShaderData* dataObject, const ShaderType& type = ShaderType::NONE) {
            return dataObject->m_ShaderSources.at(type);
        }

        inline static const STL::string& GetName(ShaderData* dataObject) { return dataObject->m_ShaderName; }
        inline static const STL::string& GetFilepath(ShaderData* dataObject) { return dataObject->m_Filepath; }

    public:
        template<typename T>
        inline static T* Cast(ShaderData* dataObject) {
            static_assert(STL::is_base_of<ShaderData, T>::value, "'T' should be derived from ShaderData.");
            return static_cast<T*>(dataObject);
        }

    protected:
        inline static STL::delegate<void(ShaderData*)> m_BindFunction;
        inline static STL::delegate<void(ShaderData*)> m_ReloadFunction;
        inline static STL::delegate<void(ShaderData*)> m_CleanFunction;
        inline static STL::delegate<void(ShaderData*)> m_UnbindFunction;
    };
}  // namespace Sentinel