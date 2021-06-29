#pragma once

#include "Sentinel/Base/PlatformDetection.h"

#ifdef ST_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif // !NOMINMAX

#endif // ST_PLATFORM_WINDOWS

#include <iostream>
#include <thread>

#include <filesystem>

#include <sstream>

#include "Sentinel/Base/Define.h"

#ifdef ST_PLATFORM_WINDOWS
#include <Windows.h>
#endif // ST_PLATFORM_WINDOWS
