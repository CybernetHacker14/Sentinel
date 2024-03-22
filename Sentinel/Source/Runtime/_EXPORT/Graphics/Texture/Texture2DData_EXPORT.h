#ifndef _EXPORT_GRAPHICS_TEXTURE_TEXTURE2DDATA_EXPORT_H
#define _EXPORT_GRAPHICS_TEXTURE_TEXTURE2DDATA_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef enum TextureWrapMode { REPEAT = 0, CLAMP = 1, MIRROR = 2, MIRROR_ONCE = 3 } TextureWrapMode;

typedef enum ShaderType ShaderType;

// Instead of adding more parameters to functions, add more variables here
typedef struct Texture2DImportSettings {
    TextureWrapMode wrapMode;
    int ReadWriteEnabled;
    int sRGB;
} Texture2DImportSettings;

typedef struct Texture2DData {
    void* native;
    void* pixels;
    TextureWrapMode wrapMode;
    ShaderType bindType;
    unsigned short width, height;
    unsigned short bindSlot;
    // This is a variable to hold all sort of boolean properties.
    // 0 - read/write enabled, 1 - hdr, 2 - sRGB
    unsigned short properties;
} Texture2DData;

#endif  // !_EXPORT_GRAPHICS_TEXTURE_TEXTURE2DDATA_EXPORT_H
