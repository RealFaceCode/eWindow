#pragma once

#include "window.hpp"

namespace ewin
{
    bool Init();
    void Terminate();
    void HideTerminal();
    void ShowTerminal();
    void CloseTerminal();
}