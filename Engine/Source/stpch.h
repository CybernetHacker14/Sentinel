#pragma once

#include "Sentinel/Common/Core/PlatformDetection.h"

#ifdef ST_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif // !NOMINMAX

#endif // ST_PLATFORM_WINDOWS

#include <iostream>
#include <sstream>
#include <fstream>

#include <thread>

#include "Sentinel/Common/Common.h"

#ifdef ST_PLATFORM_WINDOWS
#include <Windows.h>
#endif // ST_PLATFORM_WINDOWS
