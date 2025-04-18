#pragma once

#include "easywindow/defines.hpp"
#include "easywindow/wsettings.hpp"
#include "easywindow/winput.hpp"
#include "easywindow/inputhandle.hpp"

#include <filesystem>
#include <vector>
#include <unordered_map>
#include <memory>

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWimage;
struct GLFWcursor;

namespace ewin
{
    struct Window;

    EWIN_API std::shared_ptr<InputHandleKM> GetInputHandle();
    EWIN_API std::shared_ptr<InputHandleW> GetWindowInputHandle();
    EWIN_API GLFWwindow* GetCurrentContext();
    EWIN_API Window* GetCurrentWindow();

    struct EWIN_API Window
    {
    public:
        Window();
        Window(int width, int height, const char* title);
        Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
        ~Window();

        bool build();

        void setContext();
        void setCallBacks();

        void makeBorderless(bool borderless);

        void setPos(int x, int y);
        void setSize(int width, int height);
        void setAspectRatio(int numer, int denom);
        void setTitle(const char* title, bool changeSettings = true);
        void setOpacity(float opacity);
        void setIcon(const std::filesystem::path& path);
        void addCursor(std::string_view name, const std::filesystem::path& path, int xhot, int yhot);
        void setCursor(const std::string& name);

        void setWinPosWithMouse(MButton butten, int boundyX, int boundyY, int boundyWidth, int boundyHeight);

        void update();
        void close();
        bool active() const;

        GLFWwindow* getGLFWwindow();
        WindowSettings& getSettings();

        void iconify()const;
        void restore()const;
        void maximize() const;
        void show() const;
        void hide() const;
        void noteify() const;
        void focus() const;

        std::pair<int, int> frameBufferSize() const;
        std::pair<int, int> size() const;
        std::pair<int, int> pos() const;

        void updateFPS();
        double getFPS() const;
        double getDeltaTime() const;
        double getElapsedTime() const;
        double getAspect() const;
        

    private:
        GLFWwindow* window;
        GLFWmonitor* monitor;
        GLFWwindow* share;
        std::unordered_map<std::string, GLFWcursor*> cursorMap;
        WindowSettings settings;
        std::shared_ptr<InputHandleKM> kmHandle = nullptr;
        std::shared_ptr<InputHandleW> wHandle = nullptr;
        double lastTime;
        double deltaTime;
        double elapsedTime;
        float fps;
    };
}