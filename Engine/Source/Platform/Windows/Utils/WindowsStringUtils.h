#pragma once

#include "Sentinel/Common/Common.h"

#include <Windows.h>

namespace Sentinel
{
	namespace WindowsStringUtils
	{
		static STL::wstring TranscodeUTF8toUTF16(const char* s, Size_t nBytes) {
			STL::wstring buf(nBytes, L'x');
			Int32 reqChars = MultiByteToWideChar(CP_UTF8, 0, s, nBytes, &buf[0], buf.size());

			if (!reqChars)
				ST_ENGINE_ASSERT(false, "Failed to Transcode UTF-8 to UTF-16");

			buf.resize(reqChars);
			return buf;
		}

		static STL::wstring TranscodeUTF8toUTF16(const STL::string& s) {
			return TranscodeUTF8toUTF16(s.c_str(), s.size());
		}

		static STL::string TranscodeUTF16toUTF8(const wchar_t* s, Size_t nChars) {
			STL::string buf(nChars * 2, 'x');
			Int32 reqBytes = WideCharToMultiByte(CP_UTF8, 0, s, nChars, &buf[0], buf.size(), 0, 0);

			if (!reqBytes)
				ST_ENGINE_ASSERT(false, "Failed to Transcode UTF-16 to UTF-8");

			buf.resize(reqBytes);
			return buf;
		}

		static STL::string TranscodeUTF16toUTF8(const STL::wstring& s) {
			return TranscodeUTF16toUTF8(s.c_str(), s.size());
		}
	}
}
