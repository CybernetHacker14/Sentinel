#ifndef WINDOWS_GRAPHICS_D3D11_TEXTURE_D3D11TEXTURE2D_H
#define WINDOWS_GRAPHICS_D3D11_TEXTURE_D3D11TEXTURE2D_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct Texture2DData Texture2DData;
typedef struct Texture2DImportSettings Texture2DImportSettings;

typedef struct ID3D11Texture2D ID3D11Texture2D;
typedef struct ID3D11ShaderResourceView ID3D11ShaderResourceView;
typedef struct ID3D11SamplerState ID3D11SamplerState;

typedef struct Tex2DD3D11Native {
    ID3D11Texture2D* nativeTex;
    ID3D11ShaderResourceView* nativeSRV;
    ID3D11SamplerState* sampler;
} Tex2DD3D11Native;

void Sentinel_D3D11Texture2D_Init();

void Sentinel_D3D11Texture2D_Deinit();

void Sentinel_D3D11Texture2D_Create(Texture2DData* texture, Texture2DImportSettings* settings, CChar* filepath);

void Sentinel_D3D11Texture2D_Destroy(Texture2DData* texture);

void Sentinel_D3D11Texture2D_Bind(Texture2DData* texture, ShaderType bindType, UShort bindSlot);

void Sentinel_D3D11Texture2D_Unbind(Texture2DData* texture);

#endif  // !WINDOWS_GRAPHICS_D3D11_TEXTURE_D3D11TEXTURE2D_H
