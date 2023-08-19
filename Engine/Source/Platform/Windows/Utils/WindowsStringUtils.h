#pragma once

#include "Sentinel/Common/Strings/String.h"

namespace Sentinel {
    namespace WindowsStringUtils {
        static STL::wstring TranscodeUTF8toUTF16(const char* s, Size_t nBytes);
        /*static STL::wstring TranscodeUTF8toUTF16(const char* s, Size_t nBytes) {
            STL::wstring buf(nBytes, L'x');
            Int32 reqChars = MultiByteToWideChar(CP_UTF8, 0, s, nBytes, &buf[0], buf.size());

            if (!reqChars) ST_ENGINE_ASSERT(false, "Failed to Transcode UTF-8 to UTF-16");

            buf.resize(reqChars);
            return buf;
        }*/

        static STL::wstring TranscodeUTF8toUTF16(const String& s);
        /*static STL::wstring TranscodeUTF8toUTF16(const String& s) {
            return TranscodeUTF8toUTF16(s.C_Str(), s.Length());
        }*/

        static String TranscodeUTF16toUTF8(const wchar_t* s, Size_t nChars);
        //static String TranscodeUTF16toUTF8(const wchar_t* s, Size_t nChars) {
        //    String buf(nChars * 2, 'x');
        //    Int32 reqBytes = WideCharToMultiByte(CP_UTF8, 0, s, nChars, &buf[0], buf.Length(), 0, 0);

        //    if (!reqBytes) ST_STATIC_ASSERT(false);  // Failed to Transcode UTF-16 to UTF-8

        //    buf.resize(reqBytes);
        //    return buf;
        //}

        static String TranscodeUTF16toUTF8(const STL::wstring& s);
        /*static String TranscodeUTF16toUTF8(const STL::wstring& s) {
            return TranscodeUTF16toUTF8(s.c_str(), s.size());
        }*/
    }  // namespace WindowsStringUtils
}  // namespace Sentinel
