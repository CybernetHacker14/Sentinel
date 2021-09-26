#include "stpch.h"

#include "Sentinel/Base/STL.h"

#include <stdio.h>

#if USE_EASTL == 1
#ifndef DEFINE_OVERLOADS
#define DEFINE_OVERLOADS

int Vsnprintf8(char8_t* p, size_t n, const char8_t* pFormat, va_list arguments) {
	return vsnprintf(p, n, pFormat, arguments);
}

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned int debugFlags, const char* file, int line) {
	return new uint8_t[size];
};

void* __cdecl operator new[](size_t size, size_t alignment, size_t alignmentOffset,
	const char* pName, int flags, unsigned int debugFlags, const char* file, int line) {
		return new uint8_t[size];
};

#endif // !DEFINE_OVERLOADS
#endif // USE_EASTL == 1
