#include "easywindow/winput.hpp"

#include <fstream>
#include <eutil/buffer/Array.hpp>
#include <eutil/filesystem/FileIOGeneric.hpp>
#include <eutil/filesystem/FileLoad.hpp>

namespace ewin
{
	util::Array Drops::getContexts() const
	{
		util::Array context;
		for(auto& path : paths)
		{
			auto file = util::LoadFile(path);
			if(file.has_value())
			{
				auto& fileData = file.value();
				context.write(fileData.size());
				context.write(fileData.data(), fileData.size());
			}
		}
		return context;
	}

	EWIN_API void ResetButtons(IOKMBuffer& input)
	{
		for (int i = 0; i < expr::MaxMButton; i++)
		{
			input.buttonReset[i].state = InputState::NONE;
			input.buttonReset[i].button = 0;
			input.buttonReset[i].action = 0;
			input.buttonReset[i].mods = 0;
		}

		input.lastPressedButton.state = InputState::NONE;
		input.lastPressedButton.button = 0;
		input.lastPressedButton.action = 0;
		input.lastPressedButton.mods = 0;

		input.lastReleasedButton.state = InputState::NONE;
		input.lastReleasedButton.button = 0;
		input.lastReleasedButton.action = 0;
		input.lastReleasedButton.mods = 0;

	}

	EWIN_API void ResetKeys(IOKMBuffer& input)
	{
		for (int i = 0; i < expr::MaxKeyboard; i++)
		{
			input.keyReset[i].state = InputState::NONE;
			input.keyReset[i].key = 0;
			input.keyReset[i].scancode = 0;
			input.keyReset[i].action = 0;
			input.keyReset[i].mods = 0;
		}

		input.lastPressedKey.state = InputState::NONE;
		input.lastPressedKey.key = 0;
		input.lastPressedKey.scancode = 0;
		input.lastPressedKey.action = 0;
		input.lastPressedKey.mods = 0;

		input.lastReleasedKey.state = InputState::NONE;
		input.lastReleasedKey.key = 0;
		input.lastReleasedKey.scancode = 0;
		input.lastReleasedKey.action = 0;
		input.lastReleasedKey.mods = 0;
	}

	EWIN_API void ResetCursorEnter(IOWBuffer& input)
	{
		input.cursorEnter = CurserEnter::NONE;
	}

	EWIN_API void Reset(IOKMBuffer& iokmbuffer, IOWBuffer& iowbuffer)
	{
		for (int i = 0; i < expr::MaxMButton; i++)
		{
			iokmbuffer.buttonReset[i].state = InputState::NONE;
			iokmbuffer.buttonReset[i].button = 0;
			iokmbuffer.buttonReset[i].action = 0;
			iokmbuffer.buttonReset[i].mods = 0;
		}

		ResetButtons(iokmbuffer);

		for (int i = 0; i < expr::MaxKeyboard; i++)
		{
			iokmbuffer.keyReset[i].state = InputState::NONE;
			iokmbuffer.keyReset[i].key = 0;
			iokmbuffer.keyReset[i].scancode = 0;
			iokmbuffer.keyReset[i].action = 0;
			iokmbuffer.keyReset[i].mods = 0;
		}

		ResetKeys(iokmbuffer);

		iokmbuffer.mousePos = { 0.0, 0.0 };
		iokmbuffer.lastMousePos = { 0.0, 0.0 };
		iokmbuffer.mouseDelta = { 0.0, 0.0 };
		iokmbuffer.scroll = { 0.0, 0.0 };
		iokmbuffer.wasMouseMoved = false;
		iokmbuffer.wasKeyboardUsed = false;

		iowbuffer.drops.paths.clear();

		iowbuffer.cursorEnter = CurserEnter::NONE;
		iowbuffer.isCursorEntered = false;
		iowbuffer.isWindowFocused = false;
		iowbuffer.isWindowIconified = false;
		iowbuffer.isWindowMaximized = false;

		iowbuffer.wasResized = false;
		iowbuffer.wasMoved = false;
	}
}