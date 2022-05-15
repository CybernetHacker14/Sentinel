#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/PlatformDetection.h"

namespace Sentinel
{
	class CPUInfo {
	public:
		static UInt32 GetL1CacheLineSize();

	private:
		static Int32 HasCPUID();
		static void CalculateFeatures();
		static const char* GetCPUType();

	public:
		inline static UInt32 CPUIDFeatures[4];
		inline static UInt32 CPUIDMaxFunction = 0;
		inline static Bool OSSavesYMM = false;
	};
}
