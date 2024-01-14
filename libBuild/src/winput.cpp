#include "winput.hpp"
#include <fstream>

namespace ewin
{
	namespace structs
	{
		std::vector<std::string> Drops::getContexts() const
		{
			std::vector<std::string> contexts;

			for(auto& path : paths)
			{
				FILE* file = fopen(path.string().c_str(), "rb");
				fseek(file, 0, SEEK_END);
				long size = ftell(file);
				fseek(file, 0, SEEK_SET);

				char* buffer = new char[size + 1];
				fread(buffer, 1, size, file);
				fclose(file);

				buffer[size] = '\0';

				contexts.emplace_back(buffer);
				delete[] buffer;
			}
			return contexts;
		}
	}

	namespace internal
	{
		void ResetButtons(structs::WInput& input)
		{
			for (int i = 0; i < expr::MaxButton; i++)
			{
				input.buttonReset[i].state = enums::InputState::NONE;
				input.buttonReset[i].button = 0;
				input.buttonReset[i].action = 0;
				input.buttonReset[i].mods = 0;
			}
		}

		void ResetKeys(structs::WInput& input)
		{
			for (int i = 0; i < expr::MaxKey; i++)
			{
				input.keyReset[i].state = enums::InputState::NONE;
				input.keyReset[i].key = 0;
				input.keyReset[i].scancode = 0;
				input.keyReset[i].action = 0;
				input.keyReset[i].mods = 0;
			}
		}

		void ResetScroll(structs::Scroll& scroll)
		{
			scroll.xoffset = 0.0;
			scroll.yoffset = 0.0;
		}

		void ResetCursorEnter(structs::WInput& input)
		{
			input.cursorEnter = enums::CurserEnter::NONE;
		}

		void Reset(structs::WInput& input)
		{
			for (int i = 0; i < expr::MaxButton; i++)
			{
				input.button[i].state = enums::InputState::NONE;
				input.button[i].button = 0;
				input.button[i].action = 0;
				input.button[i].mods = 0;

				input.buttonReset[i].state = enums::InputState::NONE;
				input.buttonReset[i].button = 0;
				input.buttonReset[i].action = 0;
				input.buttonReset[i].mods = 0;
			}

			for (int i = 0; i < expr::MaxKey; i++)
			{
				input.key[i].state = enums::InputState::NONE;
				input.key[i].key = 0;
				input.key[i].scancode = 0;
				input.key[i].action = 0;
				input.key[i].mods = 0;

				input.keyReset[i].state = enums::InputState::NONE;
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

			input.cursorEnter = enums::CurserEnter::NONE;
			input.isCursorEntered = false;
			input.isWindowFocused = false;
			input.isWindowIconified = false;
			input.isWindowMaximized = false;
		}
	}
}