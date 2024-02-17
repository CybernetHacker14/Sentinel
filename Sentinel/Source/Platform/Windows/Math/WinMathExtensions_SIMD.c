#include "stpch.h"
#include "Math/MathExtensions_SIMD.h"

// SSE and SSE2 are enabled by default for Windows x64 builds

#include <intrin.h>
#include <cglm/cglm.h>

#define SHUFFLE_PARAM(x, y, z, w) (z << 6) | (y << 4) | (x << 2) | w
#define _mm_madd_ps(a, b, c)      _mm_add_ps(_mm_mul_ps((a), (b)), (c))
#define _mm_shuffle(v, n)         _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(n, n, n, n))
#define _mm_replicate_x_ps(v)     _mm_shuffle(v, 0)
#define _mm_replicate_y_ps(v)     _mm_shuffle(v, 1)
#define _mm_replicate_z_ps(v)     _mm_shuffle(v, 2)
#define _mm_replicate_w_ps(v)     _mm_shuffle(v, 3)

static Float HorizontalSumSSE3(__m128 v) {
    __m128 shuf = _mm_movehdup_ps(v);
    __m128 sums = _mm_add_ps(v, shuf);
    shuf = _mm_movehl_ps(shuf, sums);
    sums = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

Float Sentinel_MathExtensions_SIMD_SquareRoot(Float number) {
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(number)));
}

Float Sentinel_MathExtensions_SIMD_Cos(Float radians) {
    return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(radians)));
}

Float Sentinel_MathExtensions_SIMD_Sin(Float radians) {
    return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(radians)));
}

void Sentinel_MathExtensions_SIMD_CrossProduct(const vec3 a, const vec3 b, vec3 out) {
    __m128 a4, b4, result;

    a4 = _mm_setr_ps(a[0], a[1], a[2], 0.0f);
    b4 = _mm_setr_ps(b[0], b[1], b[2], 0.0f);

    // http://threadlocalmutex.com/?p=8
    __m128 a_yzx = _mm_shuffle_ps(a4, a4, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 b_yzx = _mm_shuffle_ps(b4, b4, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 c = _mm_sub_ps(_mm_mul_ps(a4, b_yzx), _mm_mul_ps(a_yzx, b4));
    result = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));

    Float resultVec4[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    _mm_storeu_ps(resultVec4, result);

    out[0] = resultVec4[0];
    out[1] = resultVec4[1];
    out[2] = resultVec4[2];
}

Float Sentinel_MathExtensions_SIMD_DotProduct(const vec3 a, const vec3 b) {
    __m128 mul, shuf, sum, a4, b4;

    a4 = _mm_set_ps(a[0], a[1], a[2], 0.0f);
    b4 = _mm_set_ps(b[0], b[1], b[2], 0.0f);
    mul = _mm_mul_ps(a4, b4);

    // Calculate sum of SSE Registers
    shuf = _mm_movehdup_ps(mul);  // Broadcast elements 3,1 to 2,0
    sum = _mm_add_ps(mul, shuf);
    shuf = _mm_movehl_ps(shuf, sum);  // High Half -> Low Half
    sum = _mm_add_ss(sum, shuf);

    // The Result is in the lower part of the SSE Register
    return _mm_cvtss_f32(sum);
}
