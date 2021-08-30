#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Core/Backend.h"

struct GLFWwindow;

namespace Sentinel
{
	template<typename T>
	class GraphicsContext; // Forward declaration for GraphicsContextBase

	struct ContextInfo {
		STL::string Vendor;
		STL::string Renderer;
		STL::string API;
		STL::string Version;
	};

	class GraphicsContextBase {
	public:
		template<typename T>
		inline GraphicsContext<T>* BaseDowncast() {
			static_assert(STL::is_base_of<GraphicsContext<T>, T>::value
				"Operation not allowed. 'T' should be a derived from GraphicsContext<T>.");
			return static_cast<GraphicsContext<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<GraphicsContext<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from GraphicsContext<T>.");
			return static_cast<T*>(this);
		}

	protected:
		GraphicsContextBase() = default;
	};

	template<typename T>
	class GraphicsContext : public GraphicsContextBase {
	public:
		inline void Init() {
			underlying().Init();
		}

		inline const ContextInfo& GetContextInfo() const { return m_ContextInfo; }

	protected:
		ContextInfo m_ContextInfo;

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
