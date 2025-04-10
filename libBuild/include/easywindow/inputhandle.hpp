#pragma once

#include "defines.hpp"
#include "winput.hpp"
#include <memory>

struct GLFWwindow;

namespace ewin
{
    class EWIN_API InputHandleKM
    {
    public:
        InputHandleKM();
        ~InputHandleKM();

        void setCallbacks(GLFWwindow* window);
        void update();

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

        bool wasMouseMoved() const;
        double getMouseX() const;
        double getMouseY() const;
        std::pair<double, double> getMousePos() const;
        std::pair<double, double> getMouseDelta() const;

        bool isScrollUp() const;
        bool isScrollDown() const;

        bool wasKeyboardUsed() const;
        const Key& getKey(Keyboard key) const;
        const Key& getKeyReset(Keyboard key) const;
        const Button& getButton(MButton button) const;
        const Button& getButtonReset(MButton button) const;
        const Key& getPressedKey() const;
        const Button& getPressedButton() const;
        const Key& getReleasedKey() const;
        const Button& getReleasedButton() const;

        void blockInput(bool block);
        bool isInputBlocked() const;

        void setKey(Keyboard key, InputState state, int scancode, int action, int mods);
        void setButton(MButton button, InputState state, int action, int mods);
        void setCursor(double xpos, double ypos);
        void setScroll(double xoffset, double yoffset);

        IOKMBuffer& get();

    private:
        IOKMBuffer input;
        bool blockInputFlag;
    };

    class EWIN_API InputHandleW
    {
    public:
        InputHandleW();
        ~InputHandleW();

        void setCallbacks(GLFWwindow* window);
        void update();

        bool isCursorEntered() const;
        bool isWindowFocused() const;
        bool isWindowIconified() const;
        bool isWindowMaximized() const;

        bool wasResized() const;
        bool wasMoved() const;

        bool hasCursorEntered() const;
        bool hasCursorLeft() const;

        bool hasDrops() const;
        Drops getDrops();

        IOWBuffer& get();

        void setSizePtr(std::shared_ptr<std::pair<int, int>> size);
        void setPosPtr(std::shared_ptr<std::pair<int, int>> pos);
    private:
        IOWBuffer input;
    };
}