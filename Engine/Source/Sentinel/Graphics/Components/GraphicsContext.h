#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Backend.h"

struct GLFWwindow;

namespace Sentinel
{
	struct ContextInfo {
		STL::string Vendor;
		STL::string Renderer;
		STL::string API;
		STL::string Version;
	};

	class GraphicsContextBase {
	public:
		template<typename T>
		inline T* Downcast() {
			static_assert(STL::is_base_of<GraphicsContextBase, T>::value,
				"Trying to Downcast a non GraphicsContextBase inherited class!!!");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class GraphicsContext : public GraphicsContextBase {
	public:
		inline void Init() {
			underlying().Init();
		}

	private:
		friend T;
		GraphicsContext() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};

	class GraphicsContextUtils {
	public:
		static Scope<GraphicsContextBase> Create(GLFWwindow* window);
	};
}
