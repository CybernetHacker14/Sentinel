#pragma once

#include "Sentinel/Base/Define.h"

struct GLFWwindow;

namespace Sentinel
{
	template<typename T>
	class GraphicsContextCRTP;

	struct ContextInfo {
		STL::string Vendor;
		STL::string Renderer;
		STL::string API;
		STL::string Version;
	};

	class GraphicsContext {
	public:
		void Init();
		const ContextInfo& GetContextInfo();

	public:
		static Scope<GraphicsContext> Create(GLFWwindow* window);

	protected:
		GraphicsContext() = default;

	private:
		template<typename T>
		inline GraphicsContextCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<GraphicsContextCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from GraphicsContextCRTP<T>.");
			return static_cast<GraphicsContextCRTP<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<GraphicsContextCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from GraphicsContextCRTP<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class GraphicsContextCRTP : public GraphicsContext {
	public:
		inline void Init() {
			underlying().Init();
		}

		inline const ContextInfo& GetContextInfo() const { return m_ContextInfo; }

	protected:
		ContextInfo m_ContextInfo;

	private:
		friend T;
		GraphicsContextCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
