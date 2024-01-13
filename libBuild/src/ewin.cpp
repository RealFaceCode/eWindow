#include "ewin.hpp"

#include <elog.hpp>
#include <GLFW/glfw3.h>

#if _WIN32
    #include <Windows.h>
    #include <ShObjIdl.h>
#endif

namespace ewin
{
    bool Init()
    {
        if (!glfwInit())
        {
            elog::Error("Failed to initialize eWin");
            return false;
        }

        return true;
    }

    void Terminate()
    {
        glfwTerminate();
    }

    void HideTerminal()
    {
        #if _WIN32
            HWND console = GetConsoleWindow();
            ShowWindow(console, SW_HIDE);
        #endif
    }

    void ShowTerminal()
    {
        #if _WIN32
            HWND console = GetConsoleWindow();
            ShowWindow(console, SW_SHOW);
        #endif
    }

    void CloseTerminal()
    {
        #if _WIN32
            FreeConsole();
        #endif
    }
}