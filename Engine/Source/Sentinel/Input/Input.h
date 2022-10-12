#pragma once

#include <glm/glm.hpp>

#include "Sentinel/Input/KeyCodes.h"
#include "Sentinel/Input/MouseCodes.h"

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

    protected:
        static Bool GetKey(KeyCode keycode);
        static Bool GetMouseButton(MouseCode mousecode);
    };
}  // namespace Sentinel
