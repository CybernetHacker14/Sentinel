#ifndef _EXPORT_FILESYSTEM_FILESYSTEMAPI_EXPORT_H
#define _EXPORT_FILESYSTEM_FILESYSTEMAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../ExportConditional.h"

ST_API int Sentinel_FilesystemAPI_ReadTextFileAtPath(const char* filepath, char* buffer, unsigned long long* outSize);

#endif  // !_EXPORT_FILESYSTEM_FILESYSTEMAPI_EXPORT_H
