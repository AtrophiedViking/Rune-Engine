#pragma once
#include <string>
#include <cstdint>
#include <functional>
#include "Event.h"

namespace Rune {

    struct WindowConfig
    {
        std::string Title;
        uint32_t Width = 1280;
        uint32_t Height = 720;

        bool Fullscreen = true;
        bool VSync = true;

        using EventCallbackFn = std::function<void(Event&)>;
        EventCallbackFn EventCallback;
    };

}