#pragma once

#include "Sentinel/Common/Common.h"

struct GLFWwindow;

namespace Sentinel
{
	struct ContextInfo {
		STL::string Vendor;
		STL::string Renderer;
		STL::string API;
		STL::string Version;
	};

	class GraphicsContext {
	public:
		inline void Init() const {
			if (!m_InitFunction)
				return;

			m_InitFunction();
		}

		inline const ContextInfo& GetContextInfo() const { return m_ContextInfo; }

	public:
		static UniqueRef<GraphicsContext> Create(GLFWwindow* window);

	protected:
		STL::delegate<void()> m_InitFunction;

	protected:
		ContextInfo m_ContextInfo;
	};
}
