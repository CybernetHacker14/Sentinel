#pragma once

#include "Sentinel/Common/Common.h"

#pragma warning(push, 0)
#include <cereal/archives/xml.hpp>
#pragma warning(pop)

namespace Sentinel
{
	template<typename T>
	class TCerealizable {
	public:
		template<class Archive>
		inline void save(Archive& archive) {
			underlying().save<Archive>(archive);
		}

		template<class Archive>
		inline void load(Archive& archive) {
			underlying().load<Archive>(archive);
		}

	private:
		friend T;
		TCerealizable() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
