#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	enum class AssetFlag : UInt16 {
		NONE = 0,
		MISSING = BIT(0),
		INVALID = BIT(1)
	};

	enum class AssetType : UInt16 {
		NONE = 0,
		TEXTFILE = 1,
		CPPFILE = 2,
		HEADERFILE = 3,
		TEXTURE = 4,
		SHADER = 5,
		MATERIAL = 6,
		SCENE = 7
	};
}
