#pragma once

#define ADD_SIMD_MATH 1

#include "Sentinel/Common/Common.h"

#if ADD_SIMD_MATH == 1
#include "Sentinel/Math/SIMD.h"
#endif // ADD_SIMD_MATH == 1

namespace Sentinel
{
	class Math {
	public:
		static Float FastInverseSquareRoot(Float number);
		static Float FastAbsolute(Float number);

		static Float FastCos(Float radians);
		static Float FastSin(Float radians);

		static glm::vec3 FastNormalize(const glm::vec3& vector);

	public:
		inline static constexpr Float PI = 3.1415926535897932f;
		inline static constexpr Float PI_HALF = 1.57079632679f;
		inline static constexpr Float TWO_PI = 6.28318530718f;
		inline static constexpr Float PI_INV = 0.31830988618f;
		inline static constexpr Float INVTWO_PI = 0.1591549f;
		inline static constexpr Float PI_SQUARED = 9.86960440109f;
		inline static constexpr Float PI_THREEHALF = 4.7123889f;
		inline static constexpr Float PI_QUARTER = 0.7853982f;
	};
}
