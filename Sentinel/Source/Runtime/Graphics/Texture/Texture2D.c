#include "stpch.h"
#include "_EXPORT/Graphics/RendererAPI_EXPORT.h"
#include "_EXPORT/Graphics/RendererData_EXPORT.h"
#include "_EXPORT/Graphics/Texture/Texture2DAPI_EXPORT.h"
#include "_EXPORT/Graphics/Texture/Texture2DData_EXPORT.h"
#include "Allocator/FixedSlabAllocator.h"
#include "Common/Core/Macros.h"
#include "Graphics/Texture/Texture2D.h"

#include <stdlib.h>

#ifdef ST_PLATFORM_WINDOWS
    #include "Graphics/D3D11/Texture/D3D11Texture2D.h"
#endif  // ST_PLATFORM_WINDOWS

typedef struct Tex2DAPI {
    void (*init)();
    void (*deinit)();
    void (*create)(Texture2DData*, Texture2DImportSettings*, CChar*);
    void (*destroy)(Texture2DData*);
    void (*bind)(Texture2DData*, ShaderType, UShort);
    void (*unbind)(Texture2DData*);
} Tex2DAPI;

static Tex2DAPI tex2DAPI;

static FixedSlabAllocator tex2DAllocator;

void Sentinel_Texture2D_Init() {
    if (Sentinel_Renderer_GetBackend() == D3D11) {
        tex2DAPI.init = Sentinel_D3D11Texture2D_Init;
        tex2DAPI.deinit = Sentinel_D3D11Texture2D_Deinit;
        tex2DAPI.create = Sentinel_D3D11Texture2D_Create;
        tex2DAPI.destroy = Sentinel_D3D11Texture2D_Destroy;
        tex2DAPI.bind = Sentinel_D3D11Texture2D_Bind;
        tex2DAPI.unbind = Sentinel_D3D11Texture2D_Unbind;
    }

    Sentinel_FixedSlabAllocator_Allocate(&tex2DAllocator, sizeof(Texture2DData), 32);
    tex2DAPI.init();
}

void Sentinel_Texture2D_Deinit() {
    Texture2DData* start = (Texture2DData*)tex2DAllocator.startingAddress;
    for (UShort i = 0; i < tex2DAllocator.max; ++i) {
        if (start[i].native != NULL) {
            tex2DAPI.unbind(&(start[i]));
            tex2DAPI.destroy(&(start[i]));
        }
    }

    tex2DAPI.deinit();

    Sentinel_FixedSlabAllocator_DeleteAll(&tex2DAllocator);
    Sentinel_FixedSlabAllocator_Deallocate(&tex2DAllocator);
}

ST_API Texture2DData* Sentinel_Texture2DAPI_CreateFromFile(const char* filepath, Texture2DImportSettings* settings) {
    UShort index = 0;
    Texture2DData* textureData = (Texture2DData*)Sentinel_FixedSlabAllocator_New(&tex2DAllocator, &index);
    textureData->wrapMode = settings->wrapMode;
    textureData->properties |= settings->ReadWriteEnabled ? ST_BIT(0) : textureData->properties;
    textureData->properties |= settings->sRGB ? ST_BIT(2) : textureData->properties;
    tex2DAPI.create(textureData, settings, filepath);
    return textureData;
}

ST_API void Sentinel_Texture2DAPI_Destroy(Texture2DData* texture) {
    tex2DAPI.destroy(texture);

    UShort index;
    Sentinel_FixedSlabAllocator_Delete(&tex2DAllocator, texture, &index);
}

ST_API void Sentinel_Texture2DAPI_Bind(Texture2DData* texture, ShaderType bindType, unsigned short bindSlot) {
    tex2DAPI.bind(texture, bindType, bindSlot);
}

ST_API void Sentinel_Texture2DAPI_Unbind(Texture2DData* texture) {
    tex2DAPI.unbind(texture);
}

ST_API int Sentinel_Texture2DAPI_IsReadWriteEnabled(Texture2DData* texture) {
    return ST_CHECK_BIT(texture->properties, 0);
}

ST_API int Sentinel_Texture2DAPI_IsHDR(Texture2DData* texture) {
    return ST_CHECK_BIT(texture->properties, 1);
}

ST_API int Sentinel_Texture2DAPI_IssRGB(Texture2DData* texture) {
    return ST_CHECK_BIT(texture->properties, 2);
}
