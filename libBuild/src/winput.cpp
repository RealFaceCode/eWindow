#include "winput.hpp"
#include <fstream>

namespace ewin
{

	std::vector<std::string> Drops::getContexts() const
	{
		std::vector<std::string> contexts;

		for(auto& path : paths)
		{
			FILE* file = ::fopen(path.string().c_str(), "rb");
			::fseek(file, 0, SEEK_END);
			long size = ftell(file);
			::fseek(file, 0, SEEK_SET);

			auto* buffer = new char[size + 1];
			::fread(buffer, 1, size, file);
			::fclose(file);

			buffer[size] = '\0';

			contexts.emplace_back(buffer);
			delete[] buffer;
		}
		return contexts;
	}

	void ResetButtons(WInput& input)
	{
		for (int i = 0; i < expr::MaxMButton; i++)
		{
			input.buttonReset[i].state = InputState::NONE;
			input.buttonReset[i].button = 0;
			input.buttonReset[i].action = 0;
			input.buttonReset[i].mods = 0;
		}
	}

	void ResetKeys(WInput& input)
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

	void ResetScroll(Scroll& scroll)
	{
		scroll.xoffset = 0.0;
		scroll.yoffset = 0.0;
	}

	void ResetCursorEnter(WInput& input)
	{
		input.cursorEnter = CurserEnter::NONE;
	}

	void Reset(WInput& input)
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