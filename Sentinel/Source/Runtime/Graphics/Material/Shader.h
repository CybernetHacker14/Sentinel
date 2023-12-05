#ifndef GRAPHICS_MATERIAL_SHADER_H
#define GRAPHICS_MATERIAL_SHADER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef enum ShaderType ShaderType;

typedef struct ShaderData {
    ShaderType type;
    void* native;
} ShaderData;

void Sentinel_Shader_Init();

void Sentinel_Shader_Deinit();

void Sentinel_Shader_Bind();

#endif  // !GRAPHICS_MATERIAL_SHADER_H
