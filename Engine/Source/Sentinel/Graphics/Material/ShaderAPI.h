#pragma once

#include "Sentinel/Graphics/Material/ShaderData.h"
#include "Sentinel/Memory/PoolAllocator.h"

namespace Sentinel {
    class ShaderAPI {
    public:
        static ShaderData* CreateShaderData(
            PoolAllocator<ShaderData>& allocator,
            ContextData* context,
            const String& filepath,
            const String& name);

        static void Bind(ShaderData* dataObject);

        static void Reload(ShaderData* dataObject);

        static void Clean(ShaderData* dataObject);

        static void Unbind(ShaderData* dataObject);

        inline static const STL::string& GetShaderSource(
            ShaderData* dataObject, const ShaderType& type = ShaderType::NONE) {
            return dataObject->m_Sources[(UInt8)type].source;
        }

        inline static const STL::string& GetName(ShaderData* dataObject) { return dataObject->m_ShaderName; }
        inline static const STL::string& GetFilepath(ShaderData* dataObject) { return dataObject->m_Filepath; }

#ifdef ST_RENDERER_DX11
        inline static ID3D10Blob* GetBinary(ShaderData* dataObject, const ShaderType type) {
            return dataObject->m_Binaries[(UInt8)type].binary;
        }
#endif  // ST_RENDERER_DX11

    private:
        static void PreprocessSource(const STL::string& source, ShaderSource* sources);
        static void CompileFromSource(ShaderData* dataObject);
        static void Load(ShaderData* dataObject);
    };
}  // namespace Sentinel