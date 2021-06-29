#include <Sentinel.h>
#include <Sentinel/Base/EntryPoint.h>

#include "ExampleLayer.h"

// For launching the application with Nvidia card if available by default
extern "C" {
	__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

class SandBox : public Sentinel::Application {
public:
	SandBox() : Sentinel::Application("Sandbox") {
		PushLayer(new ExampleLayer());
	}

	~SandBox() {}
};

Sentinel::Application* Sentinel::CreateApplication() {
	return new SandBox();
}
