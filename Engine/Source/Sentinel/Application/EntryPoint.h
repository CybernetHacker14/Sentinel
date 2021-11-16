#pragma once

#include "Sentinel/Common/Common.h"

#ifdef ST_PLATFORM_WINDOWS

// External Sentinel Application
extern Sentinel::Application* Sentinel::CreateApplication();

int main(int argc, char** argv) {
	// Standard flow - Init, Run the loop till not closed, end the application if closed

	Sentinel::Log::Init();

	auto application = Sentinel::CreateApplication();
	// TODO : R&D needed on Engine, Editor and Final Game dependency

	// OPINION @Ujjwal: Two approaches of architecture :-
	// Approach 1
	// 1. Segregate base systems like rendering(visuals, text, UI), physics, animation, audio, AI, logging, etc. in one static lib
	// 2. Segregate tool-set systems like Editor UI(viewports, panels), gizmos, in another module.
	// 3. Work some sort of logic to embed output of editor(data and logic) in a seperate executable which will use the engine module.

	// Approach 2
	// 1. Segregate base systems like rendering(visuals, text, UI). physics, animation, audio, AI, logging, etc. in one static lib
	// 2. Build the Editor in such a way that it will handle both the toolsets and the final game.
	// 3. During final build, the toolset part of the editor will be stripped away (using I guess - defines), the viewport settings will
	// be changed, and we get the final game.

	// Things to note from Discord
	// @Nekko - i would probably generally have 2 separate binaries, 1 for editor, 1 without editor. Game logic would be loaded at runtime
	// both of the binaries would come from 1 source tho, with editor specific stuff enclosed in an #ifdef block

	// @DSpider - when you hit "Play" in Unity (or Unreal), at least two things start happening: 1.) physics simulations become active, and 2.) camera changes position (and rotation) to a "camera" object in the scene
	// also, 3.) the editor UI goes away
	// 3 is easy, since you can toggle it on / off with a bool

	//OPINION2 @Ujjwal : In Ghost engine, asset management was quite a pain :-
	// So in Sentinel engine, a proper asset management system, if worked on early, should work wonders in handling data
	// in future development
	application->Run();

	delete application;
}

#endif
