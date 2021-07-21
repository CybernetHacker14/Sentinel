#pragma once

#include <glm/glm.hpp>

#include "Sentinel/Input/KeyCodes.h"
#include "Sentinel/Input/MouseCodes.h"

namespace Sentinel
{
	class Input {
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyDown(KeyCode keycode);
		static bool IsKeyUp(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static bool IsMouseButtonDown(MouseCode button);
		static bool IsMouseButtonUp(MouseCode button);

		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	protected:
		static bool GetKey(KeyCode keycode);
		static bool GetMouseButton(MouseCode mousecode);

	private:
		friend class Application;

		static void OnUpdate();
	};
}
