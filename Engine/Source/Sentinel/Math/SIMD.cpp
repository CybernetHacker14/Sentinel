#include "stpch.h"
#include "Sentinel/Math/SIMD.h"

#include <xmmintrin.h>

#define SHUFFLE_PARAM(x, y, z, w) (z<<6) | (y<<4) | (x<<2) | w
#define _mm_madd_ps(a, b, c) _mm_add_ps(_mm_mul_ps((a), (b)), (c))
#define _mm_shuffle(v, n) _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(n,n,n,n))
#define _mm_replicate_x_ps(v) _mm_shuffle(v, 0)
#define _mm_replicate_y_ps(v) _mm_shuffle(v, 1)
#define _mm_replicate_z_ps(v) _mm_shuffle(v, 2)
#define _mm_replicate_w_ps(v) _mm_shuffle(v, 3)

namespace Sentinel
{
	// Horizontal SSE Vector Sum
	static Float HorizontalSumSSE3(__m128 v) {
		__m128 shuf = _mm_movehdup_ps(v);
		__m128 sums = _mm_add_ps(v, shuf);
		shuf = _mm_movehl_ps(shuf, sums);
		sums = _mm_add_ss(sums, shuf);
		return _mm_cvtss_f32(sums);
	}

	Float SIMDMath::SSESquareRoot(Float number) {
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(number)));
	}

	Float SIMDMath::SSECos(Float radians) {
		return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(radians)));
	}

	Float SIMDMath::SSESin(Float radians) {
		return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(radians)));
	}

	glm::mat4 SIMDMath::SSEMatrixMultiply(const glm::mat4& a, const glm::mat4& b) {
		__m128 m2_row_0 = _mm_setr_ps(a[0][0], a[0][1], a[0][2], a[0][3]);
		__m128 m2_row_1 = _mm_setr_ps(a[1][0], a[1][1], a[1][2], a[1][3]);
		__m128 m2_row_2 = _mm_setr_ps(a[2][0], a[2][1], a[2][2], a[2][3]);
		__m128 m2_row_3 = _mm_setr_ps(a[3][0], a[3][1], a[3][2], a[3][3]);

		__m128 m1_row_0 = _mm_setr_ps(b[0][0], b[0][1], b[0][2], b[0][3]);
		__m128 m1_row_1 = _mm_setr_ps(b[1][0], b[1][1], b[1][2], b[1][3]);
		__m128 m1_row_2 = _mm_setr_ps(b[2][0], b[2][1], b[2][2], b[2][3]);
		__m128 m1_row_3 = _mm_setr_ps(b[3][0], b[3][1], b[3][2], b[3][3]);

		__m128 out0, out1, out2, out3;

		out0 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_0));
		out1 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_1));
		out2 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_2));
		out3 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_3));

		out0 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_3), out3);

		out0 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_3), out3);

		out0 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_3), out3);

		glm::mat4 dest = glm::mat4(0.0f);

		_mm_store_ps(&dest[0][0], out0);
		_mm_store_ps(&dest[1][0], out1);
		_mm_store_ps(&dest[2][0], out2);
		_mm_store_ps(&dest[3][0], out3);

		return dest;
	}

	glm::vec3 SIMDMath::SSECrossProduct(const glm::vec3& a, const glm::vec3& b) {
		__m128 a4, b4, result;

		a4 = _mm_setr_ps(a.x, a.y, a.z, 0.0f);
		b4 = _mm_setr_ps(b.x, b.y, b.z, 0.0f);

		// http://threadlocalmutex.com/?p=8
		__m128 a_yzx = _mm_shuffle_ps(a4, a4, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 b_yzx = _mm_shuffle_ps(b4, b4, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 c = _mm_sub_ps(_mm_mul_ps(a4, b_yzx), _mm_mul_ps(a_yzx, b4));
		result = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));

		float resultVec4[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		_mm_storeu_ps(resultVec4, result);

		return glm::vec3(resultVec4[0], resultVec4[1], resultVec4[2]);
	}

	Float SIMDMath::SSEDotProduct(const glm::vec3& a, const glm::vec3& b) {
		__m128 mulRes, shufReg, sumReg, a4, b4;

		a4 = _mm_set_ps(a.x, a.y, a.z, 0.0f);
		b4 = _mm_set_ps(b.x, b.y, b.z, 0.0f);
		mulRes = _mm_mul_ps(a4, b4);

		// Calculate sum of SSE Registers
		shufReg = _mm_movehdup_ps(mulRes);        // Broadcast elements 3,1 to 2,0
		sumReg = _mm_add_ps(mulRes, shufReg);
		shufReg = _mm_movehl_ps(shufReg, sumReg); // High Half -> Low Half
		sumReg = _mm_add_ss(sumReg, shufReg);

		// The Result is in the lower part of the SSE Register
		return _mm_cvtss_f32(sumReg);
	}

	Float SIMDMath::SSEDistanceSquare(const glm::vec3& a, const glm::vec3& b) {
		__m128 a4 = _mm_set_ps(a.x, a.y, a.z, 0.0f);
		__m128 b4 = _mm_set_ps(b.x, b.y, b.z, 0.0f);

		__m128 diff = _mm_sub_ps(b4, a4);
		__m128 sqr = _mm_mul_ps(diff, diff);

		return HorizontalSumSSE3(sqr);
	}
}
