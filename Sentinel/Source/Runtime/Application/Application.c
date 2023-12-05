#include "stpch.h"
#include "Application/Application.h"
#include "_EXPORT/Application/ApplicationAPI_EXPORT.h"

#define ST_ENGINE_VERSION "0.0.02"

static ApplicationConfig appConfig;

ST_API void Sentinel_CreateApplicationConfig(const char* name, const char* version) {
    appConfig.ApplicationName = name;        // Check for memcpy usage
    appConfig.ApplicationVersion = version;  // Check for memcpy usage
    appConfig.EngineName = "Sentinel";
    appConfig.EngineVersion = ST_ENGINE_VERSION;
}

ST_API const char* Sentinel_GetApplicationConfigName() {
    return appConfig.ApplicationName;
}
