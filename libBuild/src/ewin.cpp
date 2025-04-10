#include "ewin.hpp"
#include <logcraft/lc.hpp>
#include "contextsettings.hpp"
#include <GLFW/glfw3.h>

#if _WIN32
    #include <Windows.h>
#endif

namespace ewin
{
    static ContextSettings ContextSetting = {};

    EWIN_API bool Init()
    {
        lc::Init();

        if (!::glfwInit())
        {
            lc::Log<"EWIN">("ERROR", "Failed to initialize GLFW");
            return false;
        }

        SetOpenGLVersion(4, 6);
        SetOpenGLProfile(OpenGLProfile::OpenGLProfileCore);
        SetOpenGLForwardCompat(true);
        SetOpenGLDebugContext(false);
        SetOpenGLRobustness(false);

        SetBits(8, 8, 8, 8);
        SetDepthBits(24);
        SetStencilBits(8);
        SetSamples(4);
        SetSRGB(true);
        SetDoubleBuffer(true);

        return true;
    }

    EWIN_API void Terminate()
    {
        lc::DeInit();
        ::glfwTerminate();
    }

    EWIN_API void HideTerminal()
    {
        #if _WIN32
            HWND console = ::GetConsoleWindow();
            ::ShowWindow(console, SW_HIDE);
        #endif
    }

    EWIN_API void ShowTerminal()
    {
        #if _WIN32
            HWND console = ::GetConsoleWindow();
            ::ShowWindow(console, SW_SHOW);
        #endif
    }

    EWIN_API void CloseTerminal()
    {
        #if _WIN32
            ::FreeConsole();
        #endif
    }

    EWIN_API void SetOpenGLVersion(int major, int minor)
    {
        ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

        ContextSetting.major = major;
        ContextSetting.minor = minor;
    }

    EWIN_API void SetOpenGLProfile(OpenGLProfile profile)
    {

        switch (profile)
        {
        case OpenGLProfile::OpenGLProfileCore:
            ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            break;
        case OpenGLProfile::OpenGLProfileCompat:
            ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
            break;
        case OpenGLProfile::OpenGLProfileAny:
            ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
            break;
        default:
            ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            break;
        }

        ContextSetting.profile = profile;
    }

    EWIN_API void SetOpenGLForwardCompat(bool forward)
    {
        ::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forward ? GLFW_TRUE : GLFW_FALSE);

        ContextSetting.forwardCompat = forward;
    }

    EWIN_API void SetOpenGLDebugContext(bool debug)
    {
        ::glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug ? GLFW_TRUE : GLFW_FALSE);

        ContextSetting.debugContext = debug;
    }

    EWIN_API void SetOpenGLRobustness(bool robust)
    {
        ::glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, robust ? GLFW_LOSE_CONTEXT_ON_RESET : GLFW_NO_RESET_NOTIFICATION);

        ContextSetting.robustness = robust;
    }

    EWIN_API void SetBits(int red, int green, int blue, int alpha)
    {
        ::glfwWindowHint(GLFW_RED_BITS, red);
        ::glfwWindowHint(GLFW_GREEN_BITS, green);
        ::glfwWindowHint(GLFW_BLUE_BITS, blue);
        ::glfwWindowHint(GLFW_ALPHA_BITS, alpha);

        ContextSetting.redBits = red;
        ContextSetting.greenBits = green;
        ContextSetting.blueBits = blue;
        ContextSetting.alphaBits = alpha;
    }

    EWIN_API void SetDepthBits(int depth)
    {
        ::glfwWindowHint(GLFW_DEPTH_BITS, depth);

        ContextSetting.depthBits = depth;
    }

    EWIN_API void SetStencilBits(int stencil)
    {
        ::glfwWindowHint(GLFW_STENCIL_BITS, stencil);

        ContextSetting.stencilBits = stencil;
    }

    EWIN_API void SetSamples(int samples)
    {
        ::glfwWindowHint(GLFW_SAMPLES, samples);

        ContextSetting.samples = samples;
    }

    EWIN_API void SetSRGB(bool srgb)
    {
        ::glfwWindowHint(GLFW_SRGB_CAPABLE, srgb ? GLFW_TRUE : GLFW_FALSE);

        ContextSetting.srgb = srgb;
    }

    EWIN_API void SetDoubleBuffer(bool doubleBuffer)
    {
        ::glfwWindowHint(GLFW_DOUBLEBUFFER, doubleBuffer ? GLFW_TRUE : GLFW_FALSE);

        ContextSetting.doubleBuffer = doubleBuffer;
    }

    EWIN_API void SetVSync(bool vsync)
    {
        ::glfwSwapInterval(vsync ? 1 : 0);

        ContextSetting.vsync = vsync;
    }

    EWIN_API ContextSettings& GetContextSettings()
    {
        return ContextSetting;
    }

    EWIN_API loadproc GetProcAddress(const char* name)
    {
        return reinterpret_cast<loadproc>(::glfwGetProcAddress(name));
    }
}