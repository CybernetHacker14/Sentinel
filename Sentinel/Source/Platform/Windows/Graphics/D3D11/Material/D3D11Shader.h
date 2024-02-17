#ifndef WINDOWS_GRAPHICS_D3D11_MATERIAL_D3D11SHADER_H
#define WINDOWS_GRAPHICS_D3D11_MATERIAL_D3D11SHADER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct ShaderData ShaderData;

void Sentinel_D3D11Shader_Create(ShaderData* shader, CChar* source, ULLong length);

void Sentinel_D3D11Shader_Destroy(ShaderData* shader);

void Sentinel_D3D11Shader_Bind(ShaderData* shader);

#endif  // !WINDOWS_GRAPHICS_D3D11_MATERIAL_D3D11SHADER_H
