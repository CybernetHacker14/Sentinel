#include "stpch.h"
#include "Sentinel/Math/Math.h"

namespace Sentinel
{
	static Float CosInternal(Float x) {
		const float c1 = 0.9999932946f;
		const float c2 = -0.4999124376f;
		const float c3 = 0.0414877472f;
		const float c4 = -0.0012712095f;
		float x2;      // The input argument squared
		x2 = x * x;
		return (c1 + x2 * (c2 + x2 * (c3 + c4 * x2)));
	}

	Float Math::FastInverseSquareRoot(Float number) {
		// Courtesy of John Carmack, Cleve Moler, Gregory Walsh, Terje Mathison,
		// Gary Tarolli, William Kahan , K. C. Ng and the unsung hero/es behind this
		// Chris Lomont, for finding the constant 0x5f375a86, which is slightly more accurate than the original, 0x5f3759df.
		UInt32 i;
		Float x2, y;
		const Float threehalfs = 1.5f;

		x2 = number * 0.5f;
		y = number;
		i = *(Long*)&y;                        // evil floating point bit level hacking
		i = 0x5f375a86 - (i >> 1);             // what the fuck?
		y = *(Float*)&i;

		// Newton iteration
		y = y * (threehalfs - (x2 * y * y));   // 1st iteration
		y = y * (threehalfs - (x2 * y * y));   // 2nd iteration
		y = y * (threehalfs - (x2 * y * y));   // 3rd iteration

		return y;
	}

	Float Math::FastAbsolute(Float number) {
		int tmp = *(int*)&number;
		tmp &= 0x7FFFFFFF;
		return *(Float*)&tmp;
	}

	Float Math::FastCos(Float radians) {
		radians = radians - floorf(radians * INVTWO_PI) * TWO_PI;
		radians = radians > 0.0f ? radians : -radians;

		if (radians < PI_HALF) return CosInternal(radians);
		if (radians < PI) return -CosInternal(PI - radians);
		if (radians < PI_THREEHALF) return -CosInternal(radians - PI);
		return CosInternal(TWO_PI - radians);
	}

	Float Math::FastSin(Float radians) {
		return FastCos(PI_HALF - radians);
	}

	glm::vec3 Math::FastNormalize(const glm::vec3& vector) {
		return vector * FastInverseSquareRoot(SIMDMath::SSEDotProduct(vector, vector));
	}
}
