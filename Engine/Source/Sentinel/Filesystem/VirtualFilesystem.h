#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	class VirtualFilesystem {
	public:
		VirtualFilesystem();
		~VirtualFilesystem();

	private:
		STL::unordered_map<STL::string, STL::vector<STL::string>> m_MountPoints;

	private:
		static VirtualFilesystem* s_Instance;
	};
}
