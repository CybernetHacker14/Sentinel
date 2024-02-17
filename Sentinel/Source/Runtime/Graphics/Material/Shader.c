#include "stpch.h"
#include "_EXPORT/Graphics/Material/ShaderAPI_EXPORT.h"
#include "_EXPORT/Graphics/Material/ShaderDATA_EXPORT.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Common/Core/DataTypes.h"
#include "Graphics/Material/Shader.h"

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/Material/D3D11Shader.h"
#endif  // ST_PLATFORM_WINDOWS

#include <stdlib.h>

typedef struct ShaderAPI {
    void (*create)(ShaderData*, const char*, unsigned long long);
    void (*destroy)(ShaderData*);
    void (*bind)(ShaderData*);
} ShaderAPI;

static ShaderAPI shaderAPI;

static FixedSlabAllocator shaderAllocator;

void Sentinel_Shader_Init() {
    if (Sentinel_Renderer_GetBackend() == D3D11) {
        shaderAPI.create = Sentinel_D3D11Shader_Create;
        shaderAPI.destroy = Sentinel_D3D11Shader_Destroy;
        shaderAPI.bind = Sentinel_D3D11Shader_Bind;
    }

    Sentinel_FixedSlabAllocator_Allocate(&shaderAllocator, sizeof(ShaderData), 32);
}

void Sentinel_Shader_Deinit() {
    ShaderData* start = (ShaderData*)(shaderAllocator.startingAddress);
    for (UShort i = 0; i < shaderAllocator.max; ++i)
        if (start[i].native != NULL) shaderAPI.destroy(&(start[i]));

    Sentinel_FixedSlabAllocator_DeleteAll(&shaderAllocator);
    Sentinel_FixedSlabAllocator_Deallocate(&shaderAllocator);
}

void Sentinel_Shader_Bind() {
    ShaderData* start = (ShaderData*)(shaderAllocator.startingAddress);
    for (UShort i = 0; i < shaderAllocator.max; ++i)
        if (start[i].native != NULL) shaderAPI.bind(&(start[i]));
}

ST_API void Sentinel_ShaderAPI_CreateFromSource(const char* source, unsigned long long length, ShaderType type) {
    UShort index = 0;
    ShaderData* shaderData = (ShaderData*)Sentinel_FixedSlabAllocator_New(&shaderAllocator, &index);
    shaderData->type = type;
    shaderAPI.create(shaderData, source, length);
}

ST_API void Sentinel_ShaderAPI_CreateFromFile(const char* filepath, ShaderType type) {
}
