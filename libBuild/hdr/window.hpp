#pragma once

#include <filesystem>
#include <vector>
#include <unordered_map>

#include "wsettings.hpp"
#include "winput.hpp"

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWimage;
struct GLFWcursor;

namespace ewin
{
    struct Window
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
        void setTitle(const char* title);
        void setOpacity(float opacity);
        void setIcon(std::filesystem::path path);
        void addCursor(std::string_view name, std::filesystem::path path, int xhot, int yhot);
        void setCursor(const std::string& name);

        void update();
        void close();
        bool active() const;

        GLFWwindow* getGLFWwindow();
        structs::WindowSettings& getSettings();
        structs::WInput& getInput();

        bool isCursorEntered() const;
        bool isWindowFocused() const;
        bool isWindowIconified() const;
        bool isWindowMaximized() const;

        bool isKeyPressed(enums::Key key) const;
        bool isKeyReleased(enums::Key key) const;
        bool isKeyRepeated(enums::Key key) const;
        bool wasKeyPressed(enums::Key key) const;
        bool wasKeyReleased(enums::Key key) const;

        bool isButtonPressed(enums::Button button) const;
        bool isButtonReleased(enums::Button button) const;
        bool isButtonRepeated(enums::Button button) const;
        bool wasButtonPressed(enums::Button button) const;
        bool wasButtonReleased(enums::Button button) const;

        bool hasCursorEntered() const;
        bool hasCursorLeft() const;

        double getCursorX() const;
        double getCursorY() const;

        bool isScrollUp() const;
        bool isScrollDown() const;

        structs::Drops getDrops() const;

        const structs::Key& getKey(enums::Key key) const;
        const structs::Key& getKeyReset(enums::Key key) const;
        const structs::Button& getButton(enums::Button button) const;
        const structs::Button& getButtonReset(enums::Button button) const;

        void iconify()const;
        void restore()const;
        void maximize() const;
        void show() const;
        void hide() const;
        void noteify() const;
        void focus() const;

        void blockInput(bool block);
        void setKey(enums::Key key, enums::InputState state, int scancode, int action, int mods);
        void setButton(enums::Button button, enums::InputState state, int action, int mods);
        void setCursor(double xpos, double ypos);
        void setScroll(double xoffset, double yoffset);

        bool isInputBlocked() const;

    private:
        GLFWwindow* window;
        GLFWmonitor* monitor;
        GLFWwindow* share;
        std::unordered_map<std::string, GLFWcursor*> cursorMap;
        structs::WindowSettings settings;
        structs::WInput input;
        bool blockInputFlag;
    };
}