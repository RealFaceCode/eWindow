#include "easywindow/window.hpp"

#include "easywindow/imageload.hpp"

#include <GLFW/glfw3.h>
#include <logcraft/log.hpp>

namespace ewin
{
	static std::shared_ptr<InputHandleKM> KM_IHandle = nullptr;
	static std::shared_ptr<InputHandleW> W_IHandle = nullptr;
	static GLFWwindow* currentContext = nullptr;
	static Window* currentWindow = nullptr;

	EWIN_API std::shared_ptr<InputHandleKM> GetInputHandle()
	{
		return KM_IHandle;
	}

	EWIN_API std::shared_ptr<InputHandleW> GetWindowInputHandle()
	{
		return W_IHandle;
	}

	EWIN_API GLFWwindow* GetCurrentContext()
	{
		return currentContext;
	}

	EWIN_API Window* GetCurrentWindow()
	{
		return currentWindow;
	}

	static void SetInputHandleKM(std::shared_ptr<InputHandleKM> handle)
	{
		KM_IHandle = handle;
	}

	static void SetInputHandleW(std::shared_ptr<InputHandleW> handle)
	{
		W_IHandle = handle;
	}

	static void SetCurrentContext(GLFWwindow* window)
	{
		currentContext = window;
	}

	static void SetCurrentWindow(Window& window)
	{
		currentWindow = &window;
	}

    Window::Window()
	: window(nullptr), monitor(nullptr), share(nullptr), cursorMap({})
    {
        settings = 
		{
			.size = std::make_shared<std::pair<int, int>>(0, 0),
			.pos = std::make_shared<std::pair<int, int>>(0, 0),
			.title = "",
			.opacity = 1.0f
		};
    }

    Window::Window(int width, int height, const char* title)
	: window(nullptr), monitor(nullptr), share(nullptr), cursorMap({})
	{
		settings =
		{
			.size = std::make_shared<std::pair<int, int>>(width, height),
			.pos = std::make_shared<std::pair<int, int>>(0, 0),
			.title = title,
			.opacity = 1.0f
		};
    }

    Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	: window(nullptr), monitor(monitor), share(share), cursorMap({})
	{
		settings =
		{
			.size = std::make_shared<std::pair<int, int>>(width, height),
			.pos = std::make_shared<std::pair<int, int>>(0, 0),
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

        window = ::glfwCreateWindow(settings.size->first, settings.size->second, settings.title.c_str(), monitor, share);
		if (!window)
		{
			lc::Log<"EWIN">("ERROR", "Failed to create window with title: {}, size: {}x{}", settings.title, settings.size->first, settings.size->second);
			return false;
		}

		auto vidMode = ::glfwGetVideoMode(::glfwGetPrimaryMonitor());
		if(vidMode != nullptr)
		{
			int xPos = (vidMode->width - settings.size->first) / 2;
			int yPos = (vidMode->height - settings.size->second) / 2;
			setPos(xPos, yPos);
		}

		setContext();
		return true;
    }

    void Window::setContext()
    {
        ::glfwMakeContextCurrent(window);
        ::glfwSetWindowUserPointer(window, this);
		SetCurrentContext(window);
		SetCurrentWindow(*this);

		kmHandle = std::make_shared<InputHandleKM>();
		wHandle = std::make_shared<InputHandleW>();
		Reset(kmHandle->get(), wHandle->get());

		wHandle->setSizePtr(settings.size);
		wHandle->setPosPtr(settings.pos);

		setCallBacks();

		auto& fb = wHandle->get().framebufferSize;
		glfwGetFramebufferSize(window, &fb.first, &fb.second);

		SetInputHandleKM(kmHandle);
		SetInputHandleW(wHandle);
    }

    void Window::setCallBacks()
    {
		kmHandle->setCallbacks(window);
		wHandle->setCallbacks(window);
    }
	
	void Window::makeBorderless(bool borderless)
	{
		::glfwSetWindowAttrib(window, GLFW_DECORATED, borderless ? GLFW_FALSE : GLFW_TRUE);
	}

	void Window::setPos(int x, int y)
	{
		::glfwSetWindowPos(window, x, y);
		settings.pos->first = x;
		settings.pos->second = y;
	}

	void Window::setSize(int width, int height)
	{
		::glfwSetWindowSize(window, width, height);
		settings.size->first = width;
		settings.size->second = height;
	}

	void Window::setAspectRatio(int numer, int denom)
	{
		::glfwSetWindowAspectRatio(window, numer, denom);
		::glfwGetWindowSize(window, &settings.size->first, &settings.size->second);
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

	void Window::setWinPosWithMouse(MButton butten, int boundryX, int boundryY, int boundryWidth, int boundryHeight)
	{
		static int oldMX;
		static int oldMY;
		static bool first = true;
		static bool locked = false;
		static bool pressedBeforeBoundry = false;

		if(kmHandle->isButtonReleased(butten) && !first)
		{
			first = true;
			locked = false;
		}

		if(kmHandle->isButtonReleased(butten) && pressedBeforeBoundry)
			pressedBeforeBoundry = false;

		if(kmHandle->isButtonPressed(butten) && !pressedBeforeBoundry)
		{
			auto [mx, my] = static_cast<std::pair<int, int>>(kmHandle->getMousePos());

			if(!locked)
			{
				if(mx < boundryX || mx > boundryX + boundryWidth || my < boundryY || my > boundryY + boundryHeight)
				{
					pressedBeforeBoundry = true;
					return;
				}
				else
					locked = true;
			}
			
			if(first)
			{
				oldMX = mx;
				oldMY = my;
				first = false;
			}

			auto [wx, wy] = pos();		
			int newX = (mx - oldMX) + wx;
			int newY = (my - oldMY) + wy;

			setPos(newX, newY);
		}
	}

    void Window::update()
    {
		if(window != nullptr)
		{
			double currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;
			elapsedTime += deltaTime;

			kmHandle->update();
			wHandle->update();
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
		return !::glfwWindowShouldClose(window);
	}

    GLFWwindow* Window::getGLFWwindow()
    {
        return window;
    }

    WindowSettings& Window::getSettings()
    {
        return settings;
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


	std::pair<int, int> Window::frameBufferSize() const
	{	
		auto& buf = wHandle->get();
		return buf.framebufferSize;
	}

	std::pair<int, int> Window::size() const
	{
		return {settings.size->first, settings.size->second};
	}

	std::pair<int, int> Window::pos() const
	{
		return {settings.pos->first, settings.pos->second};
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
			fps = nbFrames / (currentTime - lastTime);
			nbFrames = 0;
			lastTime += 1.0;
		}
	}

	double Window::getFPS() const
	{
		return fps;
	}

	double Window::getDeltaTime() const
	{
		return deltaTime;
	}

	double Window::getElapsedTime() const
	{
		return elapsedTime;
	}

	double Window::getAspect() const
	{
		return settings.size->first / settings.size->second;
	}
}