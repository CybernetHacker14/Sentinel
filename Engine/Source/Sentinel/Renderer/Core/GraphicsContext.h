#pragma once

namespace Sentinel
{
	struct ContextInfo {
		unsigned char* Vendor;
		unsigned char* Renderer;
		unsigned char* API;
		unsigned char* Version;
	};

	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};
}
