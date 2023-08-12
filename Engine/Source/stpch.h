#pragma once

#include "Sentinel/Common/Core/MiscConfig.h"

#include <iostream>
#include <sstream>
#include <fstream>

#ifdef ST_PLATFORM_WINDOWS
    #include <Windows.h>
#endif  // ST_PLATFORM_WINDOWS

#ifdef ST_DEBUG
    #include "Sentinel/Logging/Log.h"
#endif  // ST_DEBUG
