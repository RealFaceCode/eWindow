#include "window.hpp"
#include <GLFW/glfw3.h>
#include <log.hpp>
#include "imageload.hpp"

namespace ewin
{
    namespace internal::callback
    {

        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			if(win->isInputBlocked())
				return;

			auto& input = win->getInput();
			input.key[key].key = key;
			input.key[key].scancode = scancode;
			input.key[key].action = action;
			input.key[key].mods = mods;

			input.keyReset[key].key = key;
			input.keyReset[key].scancode = scancode;
			input.keyReset[key].action = action;
			input.keyReset[key].mods = mods;

			switch (action)
			{
			case GLFW_PRESS:
				input.key[key].state = InputState::PRESSED;
				input.keyReset[key].state = InputState::PRESSED;
				break;
			case GLFW_RELEASE:
				input.key[key].state = InputState::RELEASED;
				input.keyReset[key].state = InputState::RELEASED;
				break;
			case GLFW_REPEAT:
				input.key[key].state = InputState::REPEATED;
				input.keyReset[key].state = InputState::REPEATED;
				break;
			}
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			if(win->isInputBlocked())
				return;

			auto& input = win->getInput();
			input.button[button].button = button;
			input.button[button].action = action;
			input.button[button].mods = mods;

			input.buttonReset[button].button = button;
			input.buttonReset[button].action = action;
			input.buttonReset[button].mods = mods;

			switch (action)
			{
			case GLFW_PRESS:
				input.button[button].state = InputState::PRESSED;
				input.buttonReset[button].state = InputState::PRESSED;
				break;
			case GLFW_RELEASE:
				input.button[button].state = InputState::RELEASED;
				input.buttonReset[button].state = InputState::RELEASED;
				break;
			}
		}

		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			if(win->isInputBlocked())
				return;

			auto& input = win->getInput();
			input.cursor.xpos = xpos;
			input.cursor.ypos = ypos;
		}

		void cursor_enter_callback(GLFWwindow* window, int entered)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			auto& input = win->getInput();
			input.isCursorEntered = entered;
			input.cursorEnter = (entered == GLFW_TRUE) ? CurserEnter::ENTERED : CurserEnter::LEFT;
		}

		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			if(win->isInputBlocked())
				return;

			auto& input = win->getInput();
			input.scroll.xoffset = xoffset;
			input.scroll.yoffset = yoffset;
		}

		void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			win->getSettings().xpos = xpos;
			win->getSettings().ypos = ypos;
			
			auto& input = win->getInput();
			input.wasMoved = true;
		}

		void window_size_callback(GLFWwindow* window, int width, int height)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			auto& settings = win->getSettings();
			settings.width = width;
			settings.height = height;

			auto& input = win->getInput();
			input.wasResized = true;
		}

		void window_close_callback(GLFWwindow* window)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			win->getSettings().width = 0;
			win->getSettings().height = 0;
		}

		void window_refresh_callback(GLFWwindow* window)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
		}

		void window_focus_callback(GLFWwindow* window, int focused)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			win->getInput().isWindowFocused = focused;
		}

		void window_iconify_callback(GLFWwindow* window, int iconified)
		{
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			auto& input = win->getInput();
			input.isWindowIconified = iconified;
			input.isWindowMaximized = false;
		}

		void window_maximize_callback(GLFWwindow* window, int maximized)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			auto& input = win->getInput();
			input.isWindowMaximized = maximized;
			input.isWindowIconified = false;
			input.wasResized = true;
		}

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			win->getSettings().fbWidth = width;
			win->getSettings().fbHeight = height;
		}

		void drop_callback(GLFWwindow* window, int count, const char** paths)
		{
			auto* win = static_cast<Window*>(::glfwGetWindowUserPointer(window));
			auto& input = win->getInput();
			input.drops.paths.clear();
			input.drops.paths.reserve(count);
			for (int i = 0; i < count; i++)
				input.drops.paths.emplace_back(paths[i]);
			input.drops.count = count;
		}
    }

    Window::Window()
	: window(nullptr), monitor(nullptr), share(nullptr), cursorMap({}), input({}), blockInputFlag(false)
    {
        settings = 
		{
			.width = 0,
			.height = 0,
			.fbWidth = 0,
			.fbHeight = 0,
			.title = "",
			.opacity = 1.0f
		};
    }

    Window::Window(int width, int height, const char* title)
	: window(nullptr), monitor(nullptr), share(nullptr), cursorMap({}), input({}), blockInputFlag(false)
	{
		settings =
		{
			.width = width,
			.height = height,
			.fbWidth = width,
			.fbHeight = height,
			.title = title,
			.opacity = 1.0f
		};
    }

    Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	: window(nullptr), monitor(monitor), share(share), cursorMap({}), input({}), blockInputFlag(false)
    {
		settings =
		{
			.width = width,
			.height = height,
			.fbWidth = width,
			.fbHeight = height,
			.title = title,
			.opacity = 1.0f
		};
    }

    Window::~Window()
    {
		for(auto& [name, cursor] : cursorMap)
			::glfwDestroyCursor(cursor);
		cursorMap.clear();
		if(window != nullptr)
		{
			::glfwDestroyWindow(window);
			window = nullptr;
		}
		if(monitor != nullptr)
			monitor = nullptr;
		if(share != nullptr)
			share = nullptr;
    }

    bool Window::build()
    {
		glfwSetErrorCallback([](int error, const char* description)
		{
			lc::Log<"EWIN">("ERROR", "GLFW Error[{}]: {}", error, description);
		});
		
        window = ::glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), monitor, share);
		if (!window)
		{
			lc::Log<"EWIN">("ERROR", "Failed to create window with title: {}, size: {}x{}", settings.title, settings.width, settings.height);
			return false;
		}

		setContext();
		return true;
    }

    void Window::setContext()
    {
        ::glfwMakeContextCurrent(window);
        ::glfwSetWindowUserPointer(window, this);
		setCallBacks();
    }

    void Window::setCallBacks()
    {
        ::glfwSetKeyCallback(window, internal::callback::key_callback);
        ::glfwSetMouseButtonCallback(window, internal::callback::mouse_button_callback);
        ::glfwSetCursorPosCallback(window, internal::callback::cursor_position_callback);
        ::glfwSetCursorEnterCallback(window, internal::callback::cursor_enter_callback);
        ::glfwSetScrollCallback(window, internal::callback::scroll_callback);
		::glfwSetWindowPosCallback(window, internal::callback::window_pos_callback);
        ::glfwSetWindowSizeCallback(window, internal::callback::window_size_callback);
        ::glfwSetWindowCloseCallback(window, internal::callback::window_close_callback);
        ::glfwSetWindowRefreshCallback(window, internal::callback::window_refresh_callback);
        ::glfwSetWindowFocusCallback(window, internal::callback::window_focus_callback);
        ::glfwSetWindowIconifyCallback(window, internal::callback::window_iconify_callback);
        ::glfwSetFramebufferSizeCallback(window, internal::callback::framebuffer_size_callback);
        ::glfwSetDropCallback(window, internal::callback::drop_callback);
    }

	void Window::setPos(int x, int y)
	{
		::glfwSetWindowPos(window, x, y);
	}

	void Window::setSize(int width, int height)
	{
		::glfwSetWindowSize(window, width, height);
		settings.width = width;
		settings.height = height;
	}

	void Window::setAspectRatio(int numer, int denom)
	{
		::glfwSetWindowAspectRatio(window, numer, denom);
		::glfwGetWindowSize(window, &settings.width, &settings.height);
	}

	void Window::setTitle(const char* title, bool changeSettings)
	{
		::glfwSetWindowTitle(window, title);
		if (changeSettings)
			settings.title = title;
		
	}

	void Window::setOpacity(float opacity)
	{
		::glfwSetWindowOpacity(window, opacity);
		settings.opacity = opacity;
	}

	void Window::setIcon(const std::filesystem::path& path)
	{
		GLFWimage image;
		auto img = LoadImage(path);
		if(!img.has_value())
		{
			lc::Log<"EWIN">("ERROR", "Failed to set Icon: {}", path.string());
			return;
		}
		image = img.value();

		::glfwSetWindowIcon(window, 1, &image);
		FreeImage(image);
	}

	void Window::addCursor(std::string_view name, const std::filesystem::path& path, int xhot, int yhot)
	{
		GLFWimage image;
		auto img = LoadImage(path);
		if(!img.has_value())
			return;
		image = img.value();

		GLFWcursor* cursor = ::glfwCreateCursor(&image, xhot, yhot);
		if(cursor == nullptr)
		{
			FreeImage(image);
			lc::Log<"EWIN">("ERROR", "Failed to create cursor: {}", path.string());
			return;
		}
		FreeImage(image);

		auto [it, succes] = cursorMap.insert(std::pair<std::string_view, GLFWcursor*>(name, cursor));
		if(!succes)
			lc::Log<"EWIN">("ERROR", "Failed to add cursor: {}", path.string());
	}

	void Window::setCursor(const std::string& name)
	{
		auto it = cursorMap.find(name);
		if(it == cursorMap.end())
		{
			lc::Log<"EWIN">("ERROR", "Failed to set cursor: {}", name);
			return;
		}
		auto cursor = it->second;
		::glfwSetCursor(window, cursor);
	}

    void Window::update()
    {
		if(window != nullptr)
		{
			ResetCursorEnter(input);
			ResetScroll(input.scroll);
			ResetButtons(input);
			ResetKeys(input);
			::glfwSwapBuffers(window);
			::glfwPollEvents();
		}
    }

    void Window::close()
    {
        ::glfwSetWindowShouldClose(window, GLFW_TRUE);
		::glfwDestroyWindow(window);
		window = nullptr;
    }

	bool Window::active() const
	{
		if(window == nullptr)
			return false;
		return !glfwWindowShouldClose(window);
	}

    GLFWwindow* Window::getGLFWwindow()
    {
        return window;
    }

    WindowSettings& Window::getSettings()
    {
        return settings;
    }

    WInput& Window::getInput()
    {
        return input;
    }

	bool Window::isCursorEntered() const
	{
		return input.isCursorEntered;
	}

	bool Window::isWindowFocused() const
	{
		return input.isWindowFocused;
	}

	bool Window::isWindowIconified() const
	{
		return input.isWindowIconified;
	}

	bool Window::isWindowMaximized() const
	{
		return input.isWindowMaximized;
	}

	bool Window::isKeyPressed(Keyboard key) const
	{
		return input.key[static_cast<int>(key)].state == InputState::PRESSED;
	}

	bool Window::isKeyReleased(Keyboard key) const
	{
		return  input.key[static_cast<int>(key)].state == InputState::RELEASED 
		|| input.key[static_cast<int>(key)].state == InputState::NONE;
	}

	bool Window::isKeyRepeated(Keyboard key) const
	{
		return input.key[static_cast<int>(key)].state == InputState::REPEATED;
	}

	bool Window::wasKeyPressed(Keyboard key) const
	{
		return input.keyReset[static_cast<int>(key)].state == InputState::PRESSED;
	}

	bool Window::wasKeyReleased(Keyboard key) const
	{
		return input.keyReset[static_cast<int>(key)].state == InputState::RELEASED;
	}

	bool Window::isButtonPressed(MButton button) const
	{
		return input.button[static_cast<int>(button)].state == InputState::PRESSED;
	}

	bool Window::isButtonReleased(MButton button) const
	{
		return input.button[static_cast<int>(button)].state == InputState::RELEASED
		|| input.button[static_cast<int>(button)].state == InputState::NONE;
	}

	bool Window::isButtonRepeated(MButton button) const
	{
		return input.button[static_cast<int>(button)].state == InputState::REPEATED;
	}

	bool Window::wasButtonPressed(MButton button) const
	{
		return input.buttonReset[static_cast<int>(button)].state == InputState::PRESSED;
	}

	bool Window::wasButtonReleased(MButton button) const
	{
		return input.buttonReset[static_cast<int>(button)].state == InputState::RELEASED;
	}

	bool Window::wasResized() const
	{
		return input.wasResized;
	}

	bool Window::wasMoved() const
	{
		return input.wasMoved;
	}

	bool Window::hasCursorEntered() const
	{
		return input.cursorEnter == CurserEnter::ENTERED;
	}

	bool Window::hasCursorLeft() const
	{
		return input.cursorEnter == CurserEnter::LEFT;
	}

	double Window::getCursorX() const
	{
		return input.cursor.xpos;
	}

	double Window::getCursorY() const
	{
		return input.cursor.ypos;
	}

	bool Window::isScrollUp() const
	{
		return input.scroll.yoffset > 0;
	}

	bool Window::isScrollDown() const
	{
		return input.scroll.yoffset < 0;
	}

	bool Window::hasDrops() const
	{
		return !input.drops.paths.empty();
	}

	Drops Window::getDrops()
	{
		auto drops = input.drops;
		input.drops.paths.clear();
		return drops;
	}

	const Key& Window::getKey(Keyboard key) const
	{
		return input.key[static_cast<int>(key)];
	}

	const Key& Window::getKeyReset(Keyboard key) const
	{
		return input.keyReset[static_cast<int>(key)];
	}

	const Button& Window::getButton(MButton button) const
	{
		return input.button[static_cast<int>(button)];
	}

	const Button& Window::getButtonReset(MButton button) const
	{
		return input.buttonReset[static_cast<int>(button)];
	}

	void Window::iconify() const
	{
		::glfwIconifyWindow(window);
	}

	void Window::restore() const
	{
		::glfwRestoreWindow(window);
	}

	void Window::maximize() const
	{
		::glfwMaximizeWindow(window);
	}

	void Window::show() const
	{
		::glfwShowWindow(window);
	}

	void Window::hide() const
	{
		::glfwHideWindow(window);
	}

	void Window::noteify() const
	{
		::glfwRequestWindowAttention(window);
	}

	void Window::focus() const
	{
		::glfwFocusWindow(window);
	}

	void Window::blockInput(bool block)
	{
		blockInputFlag = block;
	}

	void Window::setKey(Keyboard key, InputState state, int scancode, int action, int mods)
	{
		auto k = static_cast<int>(key);
		input.key[k].key = k;
		input.key[k].scancode = scancode;
		input.key[k].action = action;
		input.key[k].mods = mods;
		input.key[k].state = state;
	}

	void Window::setButton(MButton button, InputState state, int action, int mods)
	{
		auto b = static_cast<int>(button);
		input.button[b].button = b;
		input.button[b].action = action;
		input.button[b].mods = mods;
		input.button[b].state = state;
	}

	void Window::setCursor(double xpos, double ypos)
	{
		input.cursor.xpos = xpos;
		input.cursor.ypos = ypos;
		glfwSetCursorPos(window, xpos, ypos);
	}

	void Window::setScroll(double xoffset, double yoffset)
	{
		input.scroll.xoffset = xoffset;
		input.scroll.yoffset = yoffset;
	}

	bool Window::isInputBlocked() const
	{
		return blockInputFlag;
	}

	std::pair<int, int> Window::getFrameBufferSize() const
	{	
		return {settings.fbWidth, settings.fbHeight};
	}

	std::pair<int, int> Window::getWindowSize() const
	{
		return {settings.width, settings.height};
	}

	std::pair<int, int> Window::getPos() const
	{
		return {settings.xpos, settings.ypos};
	}

	void Window::updateFPS()
	{
		static double lastTime = 0;
		static int nbFrames = 0;
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 0.25)
		{
			std::string title = settings.title + " | FPS: [" + std::to_string(nbFrames) + "]";
			setTitle(title.c_str(), false);
			nbFrames = 0;
			lastTime += 1.0;
		}
	}
}