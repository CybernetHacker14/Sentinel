#include "stpch.h"
#include "Sentinel/System/UUID.h"

#include <random>

namespace Sentinel
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<UInt64> s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine)) {}

	UUID::UUID(UInt64 uuid)
		: m_UUID(uuid) {}

	UUID::UUID(const UUID& other)
		: m_UUID(other.m_UUID) {}
}
