#include <Sentinel.h>
#include <Sentinel/Base/EntryPoint.h>

// For launching the application with Nvidia card if available by default
extern "C" {
	__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

class SentinelEditor : public Sentinel::Application {
public:
	SentinelEditor() : Sentinel::Application("Sentinel Engine") {}

	~SentinelEditor() {}
};

Sentinel::Application* Sentinel::CreateApplication() {
	return new SentinelEditor();
}
