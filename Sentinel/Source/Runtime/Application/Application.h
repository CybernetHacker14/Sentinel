#ifndef APPLICATION_APPLICATION_H
#define APPLICATION_APPLICATION_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct ApplicationConfig {
    CChar* ApplicationName;
    CChar* ApplicationVersion;
    CChar* EngineName;
    CChar* EngineVersion;
} ApplicationConfig;

#endif  // !APPLICATION_APPLICATION_H
