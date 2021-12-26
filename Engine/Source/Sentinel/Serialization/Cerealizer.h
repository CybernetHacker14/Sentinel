#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	template<typename Serializable>
	class Cerealizer {
	public:
		void AddToRegistry(const Serializable& serializable);
		void RemoveFromRegistry(const Serializable& serializable);

	private:
		STL::vector<Serializable> m_Registry;
	};

	template<typename Serializable>
	inline void Cerealizer<Serializable>::AddToRegistry(const Serializable& serializable) {
		m_Registry.emplace_back(serializable);
	}

	template<typename Serializable>
	inline void Cerealizer<Serializable>::RemoveFromRegistry(const Serializable& serializable) {}
}
