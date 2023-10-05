#include "stpch.h"
#include "Sentinel/Application/Application.h"

#include "Sentinel/Common/CPU/CPUInfo.h"
#include "Sentinel/Common/Strings/MemFunctions.h"

#include "Sentinel/Common/Core/Assert.h"

namespace Sentinel {
    Application* Application::s_Instance = nullptr;

    Application::Application(CChar* name) : m_Title(name) {
        ST_BREAKPOINT_ASSERT(!s_Instance, "Application instance already exist!");

        s_Instance = this;
        CPUInfo::Init();
        MemFunctions::Init();
    }
}  // namespace Sentinel
