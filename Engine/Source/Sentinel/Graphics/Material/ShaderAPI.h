#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Graphics/Material/ShaderData.h"

namespace Sentinel {
    class ShaderAPI {
    public:
        static ShaderData* CreateShaderData(
            PoolAllocator<ShaderData>& allocator,
            ContextData* context,
            const STL::string& filepath,
            const STL::string& name);

        static void Bind(ShaderData* dataObject);

        static void Reload(ShaderData* dataObject);

        static void Clean(ShaderData* dataObject);

        static void Unbind(ShaderData* dataObject);

        inline static const STL::string& GetShaderSource(
            ShaderData* dataObject, const ShaderType& type = ShaderType::NONE) {
            return dataObject->m_ShaderSources.at(type);
        }

        inline static const STL::string& GetName(ShaderData* dataObject) { return dataObject->m_ShaderName; }
        inline static const STL::string& GetFilepath(ShaderData* dataObject) { return dataObject->m_Filepath; }

#ifdef ST_RENDERER_DX11
        inline static ID3D10Blob* GetBinary(ShaderData* dataObject, const ShaderType type) {
            return dataObject->m_BinaryMap.at(type);
        }
#endif  // ST_RENDERER_DX11

    private:
        static STL::unordered_map<ShaderType, STL::string> PreprocessSource(const STL::string& source);
        static void CompileFromSource(ShaderData* dataObject);
        static void Load(ShaderData* dataObject);
    };
}  // namespace Sentinel