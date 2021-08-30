#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	class Backend {
	public:
		enum class API {
			None = 0,
			DirectX11 = 1
		};

	public:
		inline static API GetAPI() { return s_API; }

		inline static void SetAPI(API value) {
			if (value != API::DirectX11)
			{
				ST_ENGINE_ASSERT(false, "Invalid Backend API");
				return;
			}

			s_API = value;
		}

	private:
		static API s_API;
	};
}
