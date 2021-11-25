#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel
{
	template<typename T>
	class TSingleton {
	public:
		inline static T& Get() {
			static T instance{};
			return instance;
		}

	protected:
		TSingleton() {}
		~TSingleton() {}

	private:
		TSingleton(const TSingleton&) = delete;
		TSingleton& operator=(const TSingleton&) = delete;
	};
}
