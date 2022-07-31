#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    enum ContextType : UInt8 { IMMEDIATE = 0, DEFFERED = 1 };

    struct ContextInfo {
        STL::string Vendor;
        STL::string Renderer;
        STL::string API;
        STL::string Version;
    };

    struct ContextData {
    protected:
        ContextInfo m_ContextInfo;
        ContextType m_ContextType;

    private:
        friend class ContextAPI;
    };
}  // namespace Sentinel