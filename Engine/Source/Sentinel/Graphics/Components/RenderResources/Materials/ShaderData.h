#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {

    enum class ShaderType { NONE = 0, VERTEX = 1, PIXEL = 2, COMPUTE = 3 };

    struct ShaderData {
    public:
        inline ShaderData() { m_ShaderSources.reserve(4); }

    protected:
        STL::string m_ShaderName;
        STL::string m_Filepath;

        STL::unordered_map<ShaderType, STL::string> m_ShaderSources;

    private:
        friend class ShaderAPI;
    };
}  // namespace Sentinel