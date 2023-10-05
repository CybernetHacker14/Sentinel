#pragma once

#include "Sentinel/Graphics/Material/ShaderData.h"
#include "Sentinel/Memory/FixedSlabAllocator.h"

namespace Sentinel {
    class ShaderAPI {
    public:
        static ShaderData* CreateShaderData(
            FixedSlabAllocator<ShaderData>& allocator, ContextData* context, CChar* filepath, CChar* name);

        static void Bind(ShaderData* dataObject);

        static void Reload(ShaderData* dataObject);

        static void Clean(ShaderData* dataObject);

        static void Unbind(ShaderData* dataObject);

        inline static CChar* GetShaderSource(ShaderData* dataObject, const ShaderType& type) {
            return dataObject->m_Sources[(UInt8)type];
        }

        inline static CChar* GetName(ShaderData* dataObject) { return dataObject->m_ShaderName; }
        inline static CChar* GetFilepath(ShaderData* dataObject) { return dataObject->m_Filepath; }

#ifdef ST_RENDERER_DX11
        inline static ID3D10Blob* GetBinary(ShaderData* dataObject, const ShaderType type) {
            return dataObject->m_Binaries[(UInt8)type];
        }
#endif  // ST_RENDERER_DX11
    private:
        static void Load(ShaderData* dataObject);
    };
}  // namespace Sentinel