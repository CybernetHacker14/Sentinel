#ifndef _EXPORT_ENTRYPOINT_H
#define _EXPORT_ENTRYPOINT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Engine.h"
#include "Window/WindowAPI_EXPORT.h"

extern void Sentinel_Main_Stage_OnStartup();
extern void Sentinel_Main_Stage_OnMainLoop_PreBegin();

extern void Sentinel_Main_Stage_OnMainLoop_Begin();
extern void Sentinel_Main_Stage_OnMainLoop_OnUpdate();
extern void Sentinel_Main_Stage_OnMainLoop_OnRender();
extern void Sentinel_Main_Stage_OnMainLoop_End();

extern void Sentinel_Main_Stage_OnMainLoop_PostEnd();
extern void Sentinel_Main_Stage_OnMainLoop_Shutdown();

int main(int argc, char** argv) {
    Sentinel_Internal_Main_Stage_Startup(Sentinel_Main_Stage_OnStartup);

    Sentinel_Internal_Main_Stage_MainLoop_PreBegin(Sentinel_Main_Stage_OnMainLoop_PreBegin);
    while (Sentinel_IsWindowRunning()) {
        Sentinel_Internal_Main_Stage_MainLoop_Begin(Sentinel_Main_Stage_OnMainLoop_Begin);
        Sentinel_Internal_Main_Stage_MainLoop_OnUpdate(Sentinel_Main_Stage_OnMainLoop_OnUpdate);
        Sentinel_Internal_Main_Stage_MainLoop_OnRender(Sentinel_Main_Stage_OnMainLoop_OnRender);
        Sentinel_Internal_Main_Stage_MainLoop_End(Sentinel_Main_Stage_OnMainLoop_End);
    }
    Sentinel_Internal_Main_Stage_MainLoop_PostEnd(Sentinel_Main_Stage_OnMainLoop_PostEnd);

    Sentinel_Internal_Main_Stage_Shutdown(Sentinel_Main_Stage_OnMainLoop_Shutdown);
    return 1;
}

#endif  // !_EXPORT_ENTRYPOINT_H
