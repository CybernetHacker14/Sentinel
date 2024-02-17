#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

void Sentinel_Renderer_MainLoop_PreBegin();

void Sentinel_Renderer_MainLoop_Update();

void Sentinel_Renderer_MainLoop_Render();

void Sentinel_Renderer_Deinit();

#endif  // !GRAPHICS_RENDERER_H
