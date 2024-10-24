#include "ewin.hpp"


#include <GLFW/glfw3.h>

#if _WIN32
    #include <Windows.h>
#endif

namespace ewin
{
    EWIN_API bool Init()
    {
        //elog::Init();

        if (!::glfwInit())
        {
            //elog::Error("Failed to initialize eWin");
            return false;
        }

        return true;
    }

    EWIN_API void Terminate()
    {
        glfwTerminate();
    }

    EWIN_API void HideTerminal()
    {
        #if _WIN32
            HWND console = GetConsoleWindow();
            ::ShowWindow(console, SW_HIDE);
        #endif
    }

    EWIN_API void ShowTerminal()
    {
        #if _WIN32
            HWND console = GetConsoleWindow();
            ::ShowWindow(console, SW_SHOW);
        #endif
    }

    EWIN_API void CloseTerminal()
    {
        #if _WIN32
            ::FreeConsole();
        #endif
    }
}