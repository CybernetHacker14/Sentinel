#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	class Filesystem {
	public:
		static STL::string ReadFile(const STL::string& filepath);
	};
}
