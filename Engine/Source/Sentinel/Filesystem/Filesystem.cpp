#include "stpch.h"

#include "Sentinel/Filesystem/Filesystem.h"

namespace Sentinel
{
	STL::string Filesystem::ReadFile(const STL::string& filepath) {
		STL::string result;
		std::ifstream in(filepath.c_str(), std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			ULLong size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				ST_ENGINE_ERROR("Could not read from file '{0}'", filepath.c_str());
			}
		}
		else
		{
			ST_ENGINE_ERROR("Could not open file '{0}'", filepath.c_str());
		}

		return result;
	}
}
