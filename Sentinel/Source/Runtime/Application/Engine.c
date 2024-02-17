#include "stpch.h"
#include "_EXPORT/Engine.h"

#include "Common/Memory/Malloc.h"
#include "Common/Memory/MemFunctions.h"

#include "Graphics/Material/Shader.h"
#include "Graphics/Renderer.h"
#include "Window/Window.h"

#ifdef ST_DEBUG
    #include "Common/Core/Assert.h"
#endif  // ST_DEBUG

ST_API void Sentinel_Internal_Main_Stage_Startup(void (*action)()) {
    Sentinel_MemInit();
    action();
}

ST_API void Sentinel_Internal_Main_Stage_MainLoop_PreBegin(void (*action)()) {
    action();
    Sentinel_Renderer_MainLoop_PreBegin();
}

ST_API void Sentinel_Internal_Main_Stage_MainLoop_Begin(void (*action)()) {
    action();
}

ST_API void Sentinel_Internal_Main_Stage_MainLoop_OnUpdate(void (*action)()) {
    action();
    Sentinel_Window_OnUpdate();
    Sentinel_Renderer_MainLoop_Update();
}

ST_API void Sentinel_Internal_Main_Stage_MainLoop_OnRender(void (*action)()) {
    action();
    Sentinel_Renderer_MainLoop_Render();
}

ST_API void Sentinel_Internal_Main_Stage_MainLoop_End(void (*action)()) {
    action();
}

ST_API void Sentinel_Internal_Main_Stage_MainLoop_PostEnd(void (*action)()) {
    action();
    Sentinel_Renderer_Deinit();
    Sentinel_Window_DestroyWindow();
}

ST_API void Sentinel_Internal_Main_Stage_Shutdown(void (*action)()) {
    action();
#ifdef ST_DEBUG
    ST_BREAKPOINT_ASSERT(
        Sentinel_GetTotalDynamicAllocations() == 0,
        "Memory Leak. Either memory allocated using Sentinel_Malloc wasn't freed using Sentinel_Free. Or Sentinel_Free "
        "is called on NULL pointer or pointer not allocated using Sentinel_Malloc");
#endif  // ST_DEBUG
}
