#ifndef _EXPORT_APPLICATION_APPLICATIONAPI_EXPORT_H
#define _EXPORT_APPLICATION_APPLICATIONAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../ExportConditional.h"

ST_API void Sentinel_CreateApplicationConfig(const char* name, const char* version);

ST_API const char* Sentinel_GetApplicationConfigName();

#endif  // !_EXPORT_APPLICATION_APPLICATIONAPI_EXPORT_H
