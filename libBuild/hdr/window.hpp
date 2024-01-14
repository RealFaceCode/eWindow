#pragma once

#include <filesystem>
#include <vector>

#include "wsettings.hpp"
#include "winput.hpp"

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWimage;

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

        void update();
        void close();
        bool active();

        GLFWwindow* getGLFWwindow();
        structs::WindowSettings& getSettings();
        structs::WInput& getInput();

        bool isCursorEntered();
        bool isWindowFocused();
        bool isWindowIconified();
        bool isWindowMaximized();

        bool isKeyPressed(enums::Key key);
        bool isKeyReleased(enums::Key key);
        bool isKeyRepeated(enums::Key key);
        bool wasKeyPresed(enums::Key key);
        bool wasKeyReleased(enums::Key key);

        bool isButtonPressed(enums::Button button);
        bool isButtonReleased(enums::Button button);
        bool isButtonRepeated(enums::Button button);
        bool wasButtonPresed(enums::Button button);
        bool wasButtonReleased(enums::Button button);

        bool hasCursorEntered();
        bool hasCursorLeft();

        double getCursorX();
        double getCursorY();

        bool isScrollUp();
        bool isScrollDown();

        std::vector<std::filesystem::path> getDrops();

        structs::Key getKey(enums::Key key);
        structs::Key getKeyReset(enums::Key key);
        structs::Button getButton(enums::Button button);
        structs::Button getButtonReset(enums::Button button);

        void iconify();
        void restore();
        void maximize();
        void show();
        void hide();
        void noteify();
        void focus();
    
    private:
        GLFWwindow* window;
        GLFWmonitor* monitor;
        GLFWwindow* share;
        structs::WindowSettings settings;
        structs::WInput input;
    };
}