#pragma once

#include "Sentinel/Common/Common.h"

#include <glm/glm.hpp>

namespace Sentinel
{
	class SIMDMath {
	public:
		static Float SSESquareRoot(Float number);
		static Float SSECos(Float radians);
		static Float SSESin(Float radians);

		static glm::mat4 SSEMatrixMultiply(const glm::mat4& a, const glm::mat4& b);
		static glm::vec3 SSECrossProduct(const glm::vec3& a, const glm::vec3& b);
		static Float SSEDotProduct(const glm::vec3& a, const glm::vec3& b);
		static Float SSEDistanceSquare(const glm::vec3& a, const glm::vec3& b);
	};
}
