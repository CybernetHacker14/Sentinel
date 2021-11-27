#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	namespace StringUtils
	{
		STL::vector<STL::string> SplitString(const STL::string& string, const STL::string& delimiters);
		STL::vector<STL::string> SplitString(const STL::string& string, const char delimiter);
		STL::vector<STL::string> Tokenize(const STL::string& string);
		STL::vector<STL::string> GetLines(const STL::string& string);
	}
}
