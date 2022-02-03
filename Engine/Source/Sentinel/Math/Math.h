#pragma once

#include "Sentinel/Common/Common.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace Sentinel
{
	class Math {
	public:
		static void CreateCachedSin(Float cacheSize);
		static void CreateCachedCos(Float cacheSize);

		static Float FastInverseSquareRoot(Float number);
		static Float Sin(Float number);
		static Float Cos(Float number);

		static glm::mat4 SSEMatrixMultiply(const glm::mat4& a, const glm::mat4& b);
		static glm::vec3 SSECrossProduct(const glm::vec3& a, const glm::vec3& b);
		static Float SSEDotProduct(const glm::vec3& a, const glm::vec3& b);
		static Float SSEDistanceSquare(const glm::vec3& a, const glm::vec3& b);

	public:
		inline static constexpr Float PI = 3.1415926535897932f;
		inline static constexpr Float TWO_PI = 6.28318530718f;
		inline static constexpr Float PI_INV = 0.31830988618f;

	private:
		inline static STL::unordered_map<Float, Float> s_CachedSin;
		inline static STL::unordered_map<Float, Float> s_CachedCos;
	};
}
