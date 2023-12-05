#ifndef _EXPORT_ENGINE_H
#define _EXPORT_ENGINE_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "ExportConditional.h"

ST_API void Sentinel_Internal_Main_Stage_Startup(void(*action)());
ST_API void Sentinel_Internal_Main_Stage_MainLoop_PreBegin(void (*action)());

ST_API void Sentinel_Internal_Main_Stage_MainLoop_Begin(void (*action)());
ST_API void Sentinel_Internal_Main_Stage_MainLoop_OnUpdate(void (*action)());
ST_API void Sentinel_Internal_Main_Stage_MainLoop_OnRender(void (*action)());
ST_API void Sentinel_Internal_Main_Stage_MainLoop_End(void (*action)());

ST_API void Sentinel_Internal_Main_Stage_MainLoop_PostEnd(void (*action)());
ST_API void Sentinel_Internal_Main_Stage_Shutdown(void (*action)());

#endif  // !_EXPORT_ENGINE_H
