#include "stpch.h"
#include "Sentinel/Graphics/Common/Backend.h"

namespace Sentinel {
#ifdef ST_RENDERER_DX11
    Backend::API Backend::s_API = Backend::API::DirectX11;
#endif  // ST_RENDERER_DX11

}  // namespace Sentinel
