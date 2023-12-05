#ifndef _EXPORT_GRAPHICS_TEXTURE_RENDERTEXTURE2DDATA_EXPORT_H
#define _EXPORT_GRAPHICS_TEXTURE_RENDERTEXTURE2DDATA_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef enum ColorFormat { NONE = 0, RGBA32F = 2, RGBA8UNORM = 28, R32SINT = 43 } ColorFormat;

typedef enum ShaderType ShaderType;

typedef struct RenderTexture2DData {
    void* native;
    ColorFormat colorFormat;
    ShaderType bindType;
    // This is a bool, opting to use Short instead of Int to pack the struct tightly without wasting space
    short swapchainTarget;
    unsigned short width, height;
    unsigned short bindSlot;
} RenderTexture2DData;

#endif  // !_EXPORT_GRAPHICS_TEXTURE_RENDERTEXTURE2DDATA_EXPORT_H
