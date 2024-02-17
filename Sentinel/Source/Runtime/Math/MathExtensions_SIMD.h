#ifndef MATH_MATHEXTENSIONS_SIMD_H
#define MATH_MATHEXTENSIONS_SIMD_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef float vec3[3];
typedef float vec4[4];
typedef vec4 mat4[4];

Float Sentinel_MathExtensions_SIMD_SquareRoot(Float number);

Float Sentinel_MathExtensions_SIMD_Cos(Float radians);

Float Sentinel_MathExtensions_SIMD_Sin(Float radians);

void Sentinel_MathExtensions_SIMD_CrossProduct(const vec3 a, const vec3 b, vec3 out);

Float Sentinel_MathExtensions_SIMD_DotProduct(const vec3 a, const vec3 b);

#endif  // !MATH_MATHEXTENSIONS_SIMD_H
