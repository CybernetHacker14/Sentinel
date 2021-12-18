#pragma once

#include "Sentinel/Common/Common.h"

#pragma warning(push, 0)
#include <cereal/archives/xml.hpp>
#pragma warning(pop)

namespace Sentinel
{
	// Don't mind the pun
	template<typename T>
	class TCerealizer {
	public:
		void AddToRegistry(const T& serializable) {
			m_Registry.emplace_back(serializable);
		}

		void RemoveFromRegistry(const T& serializable) {
			STL::remove(m_Registry.begin(), m_Registry.end(), serializable);
		}

	private:
		STL::vector<T> m_Registry;
	};

	template<typename T>
	inline void TCerealizer<T>::AddToRegistry(const T& serializable) {}

	template<typename T>
	inline void TCerealizer<T>::RemoveFromRegistry(const T& serializable) {}
}
