#pragma once

#include "window.hpp"
#include "defines.hpp"

typedef void (*glproc)(void);

namespace ewin
{
    EWIN_API bool Init();
    EWIN_API void Terminate();
    EWIN_API void HideTerminal();
    EWIN_API void ShowTerminal();
    EWIN_API void CloseTerminal();

    EWIN_API glproc GetProcAddress(const char* name);
}