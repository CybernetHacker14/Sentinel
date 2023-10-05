#pragma once

#include "Sentinel/Input/KeyCodes.h"
#include "Sentinel/Input/MouseCodes.h"

#include <glm/glm.hpp>

namespace Sentinel {
    class Input {
    public:
        static Bool IsKeyPressed(KeyCode keycode);
        static Bool IsKeyDown(KeyCode keycode);
        static Bool IsKeyUp(KeyCode keycode);

        static Bool IsMouseButtonPressed(MouseCode button);
        static Bool IsMouseButtonDown(MouseCode button);
        static Bool IsMouseButtonUp(MouseCode button);

        static glm::vec2 GetMousePosition();
        static Float GetMouseX();
        static Float GetMouseY();

        static void OnUpdate();
    };
}  // namespace Sentinel
