#include "stpch.h"
#include "Sentinel/Utilities/StringUtils.h"

namespace Sentinel
{
	namespace StringUtils
	{
		STL::vector<STL::string> SplitString(const STL::string& string, const STL::string& delimiters) {
			Size_t start = 0;
			Size_t end = string.find_first_of(delimiters);

			STL::vector<STL::string> result;

			while (end <= STL::string::npos)
			{
				STL::string token = string.substr(start, end - start);
				if (!token.empty())
					result.push_back(token);

				if (end == STL::string::npos)
					break;

				start = end + 1;
				end = string.find_first_of(delimiters, start);
			}

			return result;
		}

		STL::vector<STL::string> SplitString(const STL::string& string, const char delimiter) {
			return SplitString(string, STL::string(1, delimiter));
		}

		STL::vector<STL::string> Tokenize(const STL::string& string) {
			return SplitString(string, " \t\n");
		}

		STL::vector<STL::string> GetLines(const STL::string& string) {
			return SplitString(string, "\n");
		}
	}
}
