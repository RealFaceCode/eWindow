#pragma once

#include <filesystem>
#include <vector>
#include <unordered_map>
#include "wsettings.hpp"
#include "winput.hpp"
#include "defines.hpp"

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWimage;
struct GLFWcursor;

namespace ewin
{
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

        void setPos(int x, int y);
        void setSize(int width, int height);
        void setAspectRatio(int numer, int denom);
        void setTitle(const char* title, bool changeSettings = true);
        void setOpacity(float opacity);
        void setIcon(const std::filesystem::path& path);
        void addCursor(std::string_view name, const std::filesystem::path& path, int xhot, int yhot);
        void setCursor(const std::string& name);

        void update();
        void close();
        bool active() const;

        GLFWwindow* getGLFWwindow();
        WindowSettings& getSettings();
        WInput& getInput();

        bool isCursorEntered() const;
        bool isWindowFocused() const;
        bool isWindowIconified() const;
        bool isWindowMaximized() const;

        bool isKeyPressed(Keyboard key) const;
        bool isKeyReleased(Keyboard key) const;
        bool isKeyRepeated(Keyboard key) const;
        bool wasKeyPressed(Keyboard key) const;
        bool wasKeyReleased(Keyboard key) const;

        bool isButtonPressed(MButton button) const;
        bool isButtonReleased(MButton button) const;
        bool isButtonRepeated(MButton button) const;
        bool wasButtonPressed(MButton button) const;
        bool wasButtonReleased(MButton button) const;

        bool wasResized() const;
        bool wasMoved() const;

        bool hasCursorEntered() const;
        bool hasCursorLeft() const;

        double getCursorX() const;
        double getCursorY() const;

        bool isScrollUp() const;
        bool isScrollDown() const;

        bool hasDrops() const;
        Drops getDrops();

        const Key& getKey(Keyboard key) const;
        const Key& getKeyReset(Keyboard key) const;
        const Button& getButton(MButton button) const;
        const Button& getButtonReset(MButton button) const;

        void iconify()const;
        void restore()const;
        void maximize() const;
        void show() const;
        void hide() const;
        void noteify() const;
        void focus() const;

        void blockInput(bool block);
        void setKey(Keyboard key, InputState state, int scancode, int action, int mods);
        void setButton(MButton button, InputState state, int action, int mods);
        void setCursor(double xpos, double ypos);
        void setScroll(double xoffset, double yoffset);

        bool isInputBlocked() const;

        std::pair<int, int> getFrameBufferSize() const;
        std::pair<int, int> getWindowSize() const;
        std::pair<int, int> getPos() const;

        void updateFPS();
        double getFPS() const;
        double getDeltaTime() const;

    private:
        GLFWwindow* window;
        GLFWmonitor* monitor;
        GLFWwindow* share;
        std::unordered_map<std::string, GLFWcursor*> cursorMap;
        WindowSettings settings;
        WInput input;
        bool blockInputFlag;
        float fps;
    };
}