#include "inputhandle.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>

namespace ewin
{
    namespace callback
    {
#pragma region Input Callbacks
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto io = GetInputHandle();
			if(io->isInputBlocked())
				return;

            auto&  buf = io->get();
			buf.key[key].key = key;
			buf.key[key].scancode = scancode;
			buf.key[key].action = action;
			buf.key[key].mods = mods;

			buf.keyReset[key].key = key;
			buf.keyReset[key].scancode = scancode;
			buf.keyReset[key].action = action;
			buf.keyReset[key].mods = mods;

			switch (action)
			{
			case GLFW_PRESS:
				buf.key[key].state = InputState::PRESSED;
				buf.keyReset[key].state = InputState::PRESSED;
				break;
			case GLFW_RELEASE:
				buf.key[key].state = InputState::RELEASED;
				buf.keyReset[key].state = InputState::RELEASED;
				break;
			case GLFW_REPEAT:
				buf.key[key].state = InputState::REPEATED;
				buf.keyReset[key].state = InputState::REPEATED;
				break;
			}
        }

        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            auto io = GetInputHandle();
			if(io->isInputBlocked())
				return;

            auto&  buf = io->get();
			buf.button[button].button = button;
			buf.button[button].action = action;
			buf.button[button].mods = mods;

			buf.buttonReset[button].button = button;
			buf.buttonReset[button].action = action;
			buf.buttonReset[button].mods = mods;

			switch (action)
			{
			case GLFW_PRESS:
				buf.button[button].state = InputState::PRESSED;
				buf.buttonReset[button].state = InputState::PRESSED;
				break;
			case GLFW_RELEASE:
				buf.button[button].state = InputState::RELEASED;
				buf.buttonReset[button].state = InputState::RELEASED;
				break;
			}
        }

        void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
        {
            auto io = GetInputHandle();
			if(io->isInputBlocked())
				return;

            auto&  buf = io->get();
			buf.mousePos = {xpos, ypos};

            buf.wasMouseMoved = true;
        }

        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        {
            auto io = GetInputHandle();
			if(io->isInputBlocked())
				return;

            auto&  buf = io->get();
			buf.scroll = {xoffset, yoffset};
        }
#pragma endregion Input Callbacks

#pragma region Window Callbacks
        void cursor_enter_callback(GLFWwindow* window, int entered)
		{
			auto io = GetWindowInputHandle();
            auto&  buf = io->get();
			buf.isCursorEntered = entered;
			buf.cursorEnter = (entered == GLFW_TRUE) ? CurserEnter::ENTERED : CurserEnter::LEFT;
		}

		void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
		{
			auto io = GetWindowInputHandle();
            auto&  buf = io->get();
            buf.windowPos->first = xpos;
            buf.windowPos->second = ypos;
			buf.wasMoved = true;
		}

		void window_size_callback(GLFWwindow* window, int width, int height)
		{
			auto io = GetWindowInputHandle();
            auto&  buf = io->get();
            buf.windowSize->first = width;
            buf.windowSize->second = height;
            buf.wasResized = true;
		}

		void window_close_callback(GLFWwindow* window)
		{
			auto io = GetWindowInputHandle();
            auto&  buf = io->get();
            buf.windowSize->first = 0;
            buf.windowSize->second = 0;
		}

		void window_refresh_callback(GLFWwindow* window)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
		}

		void window_focus_callback(GLFWwindow* window, int focused)
		{
			auto io = GetWindowInputHandle();
            auto&  buf = io->get();
            buf.isWindowFocused = focused;
		}

		void window_iconify_callback(GLFWwindow* window, int iconified)
		{
			auto io = GetWindowInputHandle();
            auto&  buf = io->get();
			buf.isWindowIconified = iconified;
			buf.isWindowMaximized = false;
		}

		void window_maximize_callback(GLFWwindow* window, int maximized)
		{
			auto io = GetWindowInputHandle();
            auto&  buf = io->get();
			buf.isWindowMaximized = maximized;
			buf.isWindowIconified = false;
			buf.wasResized = true;
		}

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
		{
			auto io = GetWindowInputHandle();
            auto&  buf = io->get();
            buf.framebufferSize = {width, height};
		}

		void drop_callback(GLFWwindow* window, int count, const char** paths)
		{
            auto io = GetWindowInputHandle();
            auto&  buf = io->get();
			buf.drops.paths.clear();
			buf.drops.paths.reserve(count);
			for (int i = 0; i < count; i++)
				buf.drops.paths.emplace_back(paths[i]);
			buf.drops.count = count;
		}
#pragma endregion Window Callbacks
    }

#pragma region InputHandleKM
    InputHandleKM::InputHandleKM()
    : blockInputFlag(false), input()
    {}

    InputHandleKM::~InputHandleKM()
    {}

    void InputHandleKM::setCallbacks(GLFWwindow* window)
    {
        ::glfwSetKeyCallback(window, callback::key_callback);
        ::glfwSetMouseButtonCallback(window, callback::mouse_button_callback);
        ::glfwSetCursorPosCallback(window, callback::cursor_position_callback);
        ::glfwSetScrollCallback(window, callback::scroll_callback);
    }

    void InputHandleKM::update()
    {
        auto& buf = input;
        buf.wasMouseMoved = buf.mousePos != buf.lastMousePos;
        buf.mouseDelta = {buf.mousePos.first - buf.lastMousePos.first, buf.mousePos.second - buf.lastMousePos.second};
        buf.lastMousePos = buf.mousePos;

        ResetButtons(buf);
        ResetKeys(buf);

        buf.scroll = {0.0, 0.0};
        buf.wasMouseMoved = false;
    }

    bool InputHandleKM::isKeyPressed(Keyboard key) const
    {
        return input.key[static_cast<int>(key)].state == InputState::PRESSED
		|| input.key[static_cast<int>(key)].state == InputState::REPEATED;
    }

    bool InputHandleKM::isKeyReleased(Keyboard key) const
    {
        return  input.key[static_cast<int>(key)].state == InputState::RELEASED 
		|| input.key[static_cast<int>(key)].state == InputState::NONE;
    }

    bool InputHandleKM::isKeyRepeated(Keyboard key) const
    {
        return input.key[static_cast<int>(key)].state == InputState::REPEATED;
    }

    bool InputHandleKM::wasKeyPressed(Keyboard key) const
    {
        return input.keyReset[static_cast<int>(key)].state == InputState::PRESSED;
    }

    bool InputHandleKM::wasKeyReleased(Keyboard key) const
    {
        return input.keyReset[static_cast<int>(key)].state == InputState::RELEASED;
    }

    bool InputHandleKM::isButtonPressed(MButton button) const
    {
		return input.button[static_cast<int>(button)].state == InputState::PRESSED;
    }

    bool InputHandleKM::isButtonReleased(MButton button) const
    {
        return input.button[static_cast<int>(button)].state == InputState::RELEASED
		|| input.button[static_cast<int>(button)].state == InputState::NONE;
    }

    bool InputHandleKM::isButtonRepeated(MButton button) const
    {
		return input.button[static_cast<int>(button)].state == InputState::REPEATED;
    }

    bool InputHandleKM::wasButtonPressed(MButton button) const
    {
        return input.buttonReset[static_cast<int>(button)].state == InputState::PRESSED;
    }

    bool InputHandleKM::wasButtonReleased(MButton button) const
    {
		return input.buttonReset[static_cast<int>(button)].state == InputState::RELEASED;
    }

    bool InputHandleKM::wasMouseMoved() const
    {
        return input.wasMouseMoved;
    }

    double InputHandleKM::getMouseX() const
    {
        return input.mousePos.first;
    }

    double InputHandleKM::getMouseY() const
    {
        return input.mousePos.second;
    }

    std::pair<double, double> InputHandleKM::getMousePos() const
    {
        return input.mousePos;
    }

    std::pair<double, double> InputHandleKM::getMouseDelta() const
    {
        return input.mouseDelta;
    }

    bool InputHandleKM::isScrollUp() const
    {
        return input.scroll.second > 0;
    }

    bool InputHandleKM::isScrollDown() const
    {
        return input.scroll.second < 0;
    }

    const Key& InputHandleKM::getKey(Keyboard key) const
    {
        return input.key[static_cast<int>(key)];
    }

    const Key& InputHandleKM::getKeyReset(Keyboard key) const
    {
        return input.keyReset[static_cast<int>(key)];
    }

    const Button& InputHandleKM::getButton(MButton button) const
    {
        return input.button[static_cast<int>(button)];
    }

    const Button& InputHandleKM::getButtonReset(MButton button) const
    {
        return input.buttonReset[static_cast<int>(button)];
    }

    const Key& InputHandleKM::getPressedKey() const
    {

    }

    const Button& InputHandleKM::getPressedButton() const
    {

    }

    const Key& InputHandleKM::getReleasedKey() const
    {

    }

    const Button& InputHandleKM::getReleasedButton() const
    {

    }

    void InputHandleKM::blockInput(bool block)
    {
        blockInputFlag = block;
    }

    bool InputHandleKM::isInputBlocked() const
    {
        return blockInputFlag;
    }

    void InputHandleKM::setKey(Keyboard key, InputState state, int scancode, int action, int mods)
    {
        auto k = static_cast<int>(key);
		input.key[k].key = k;
		input.key[k].scancode = scancode;
		input.key[k].action = action;
		input.key[k].mods = mods;
		input.key[k].state = state;
    }

    void InputHandleKM::setButton(MButton button, InputState state, int action, int mods)
    {
		auto b = static_cast<int>(button);
		input.button[b].button = b;
		input.button[b].action = action;
		input.button[b].mods = mods;
		input.button[b].state = state;
    }

    void InputHandleKM::setCursor(double xpos, double ypos)
    {
        auto* window = GetCurrentContext();
        input.mousePos = {xpos, ypos};
        ::glfwSetCursorPos(window, xpos, ypos);
    }

    void InputHandleKM::setScroll(double xoffset, double yoffset)
    {
        input.scroll = {xoffset, yoffset};

    }

    IOKMBuffer& InputHandleKM::get()
    {
        return input;
    }

#pragma endregion InputHandleKM

#pragma region InputHandleW

    InputHandleW::InputHandleW()
    : input()
    {}

    InputHandleW::~InputHandleW()
    {}

    void InputHandleW::setCallbacks(GLFWwindow* window)
    {
        ::glfwSetCursorEnterCallback(window, callback::cursor_enter_callback);
		::glfwSetWindowPosCallback(window, callback::window_pos_callback);
        ::glfwSetWindowSizeCallback(window, callback::window_size_callback);
        ::glfwSetWindowCloseCallback(window, callback::window_close_callback);
        ::glfwSetWindowRefreshCallback(window, callback::window_refresh_callback);
        ::glfwSetWindowFocusCallback(window, callback::window_focus_callback);
        ::glfwSetWindowIconifyCallback(window, callback::window_iconify_callback);
        ::glfwSetFramebufferSizeCallback(window, callback::framebuffer_size_callback);
        ::glfwSetDropCallback(window, callback::drop_callback);
    }

    void InputHandleW::update()
    {
        auto& buf = input;
        buf.wasResized = false;
        buf.wasMoved = false;
        ResetCursorEnter(buf);
    }

    bool InputHandleW::isCursorEntered() const
    {
        return input.isCursorEntered;
    }

    bool InputHandleW::isWindowFocused() const
    {
        return input.isWindowFocused;
    }

    bool InputHandleW::isWindowIconified() const
    {
        return input.isWindowIconified; 
    }

    bool InputHandleW::isWindowMaximized() const
    {
        return input.isWindowMaximized;
    }

    bool InputHandleW::wasResized() const
    {
        return input.wasResized;
    }

    bool InputHandleW::wasMoved() const
    {
        return input.wasMoved;
    }

    bool InputHandleW::hasCursorEntered() const
    {
        return input.cursorEnter == CurserEnter::ENTERED;
    }

    bool InputHandleW::hasCursorLeft() const
    {
        return input.cursorEnter == CurserEnter::LEFT;
    }

    bool InputHandleW::hasDrops() const
    {
        return !input.drops.paths.empty();
    }

    Drops InputHandleW::getDrops()
    {
        auto drops = input.drops;
		input.drops.paths.clear();
		return drops;
    }

    IOWBuffer& InputHandleW::get()
    {
        return input;
    }

    void InputHandleW::setSizePtr(std::shared_ptr<std::pair<int, int>> size)
    {
        input.windowSize = size;
    }

    void InputHandleW::setPosPtr(std::shared_ptr<std::pair<int, int>> pos)
    {
        input.windowPos = pos;
    }

#pragma endregion InputHandleW
}