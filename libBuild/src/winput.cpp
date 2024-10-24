#include "winput.hpp"
#include <fstream>
#include <fioc.hpp>

namespace ewin
{
	std::vector<std::string> Drops::getContexts() const
	{
		std::vector<std::string> contexts;

		for(auto& path : paths)
		{
			FILE* file = nullptr;
			std::string pathStr = path.string();
			const char* cpath = pathStr.c_str();
			size_t size = 0;
			eutil::fioc::GetFileSize(cpath, size);
			auto* buffer = new char[size + 1];

			eutil::fioc::ReadDataFromFileRaw(&file, cpath, (void**)&buffer, size, size + 1);
			buffer[size] = '\0';
			eutil::fioc::close_file(&file, true);

			contexts.emplace_back(buffer);
			delete[] buffer;
		}
		return contexts;
	}

	EWIN_API void ResetButtons(WInput& input)
	{
		for (int i = 0; i < expr::MaxMButton; i++)
		{
			input.buttonReset[i].state = InputState::NONE;
			input.buttonReset[i].button = 0;
			input.buttonReset[i].action = 0;
			input.buttonReset[i].mods = 0;
		}
	}

	EWIN_API void ResetKeys(WInput& input)
	{
		for (int i = 0; i < expr::MaxKeyboard; i++)
		{
			input.keyReset[i].state = InputState::NONE;
			input.keyReset[i].key = 0;
			input.keyReset[i].scancode = 0;
			input.keyReset[i].action = 0;
			input.keyReset[i].mods = 0;
		}
	}

	EWIN_API void ResetScroll(Scroll& scroll)
	{
		scroll.xoffset = 0.0;
		scroll.yoffset = 0.0;
	}

	EWIN_API void ResetCursorEnter(WInput& input)
	{
		input.cursorEnter = CurserEnter::NONE;
	}

	EWIN_API void Reset(WInput& input)
	{
		for (int i = 0; i < expr::MaxMButton; i++)
		{
			input.button[i].state = InputState::NONE;
			input.button[i].button = 0;
			input.button[i].action = 0;
			input.button[i].mods = 0;

			input.buttonReset[i].state = InputState::NONE;
			input.buttonReset[i].button = 0;
			input.buttonReset[i].action = 0;
			input.buttonReset[i].mods = 0;
		}

		for (int i = 0; i < expr::MaxKeyboard; i++)
		{
			input.key[i].state = InputState::NONE;
			input.key[i].key = 0;
			input.key[i].scancode = 0;
			input.key[i].action = 0;
			input.key[i].mods = 0;

			input.keyReset[i].state = InputState::NONE;
			input.keyReset[i].key = 0;
			input.keyReset[i].scancode = 0;
			input.keyReset[i].action = 0;
			input.keyReset[i].mods = 0;
		}

		input.cursor.xpos = 0.0;
		input.cursor.ypos = 0.0;

		input.scroll.xoffset = 0.0;
		input.scroll.yoffset = 0.0;

		input.drops.paths.clear();

		input.cursorEnter = CurserEnter::NONE;
		input.isCursorEntered = false;
		input.isWindowFocused = false;
		input.isWindowIconified = false;
		input.isWindowMaximized = false;
	}
}