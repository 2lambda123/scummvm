/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef XEEN_DIALOGS_STRING_INPUT_H
#define XEEN_DIALOGS_STRING_INPUT_H

#include "common/keyboard.h"
#include "mm/xeen/dialogs/dialogs.h"
#include "mm/xeen/screen.h"

namespace MM {
namespace Xeen {

class Input : public ButtonContainer {
private:
	/**
	 * Draws the text input and cursor and waits until the user presses a key
	 */
	Common::KeyState waitForKey(const Common::String &msg);

	/**
	 * Animates the box text cursor
	 */
	void animateCursor();
protected:
	Window *_window;
	int _cursorAnimIndex;

	int nonEnToUpper(uint16 ascii);
	int nonEnToLower(uint16 ascii);

	/**
	 * Allows the user to enter a string
	 */
	int getString(Common::String &line, uint maxLen, int maxWidth, bool isNumeric);

	Input(XeenEngine *vm, Window *window) : ButtonContainer(vm),
		_window(window), _cursorAnimIndex(0) {}
public:
	static int show(XeenEngine *vm, Window *window, Common::String &line,
		uint maxLen, int maxWidth, bool isNumeric = false);
};

class StringInput : public Input {
protected:
	StringInput(XeenEngine *vm);

	int execute(bool type, const Common::String &expected,
		const Common::String &title, int opcode);
public:
	static int show(XeenEngine *vm, bool type, const Common::String &msg1,
		const Common::String &msg2, int opcode);
};

class NumericInput : public Input {
private:
	NumericInput(XeenEngine *vm, int window);

	int execute(int maxLength, int maxWidth);
public:
	static int show(XeenEngine *vm, int window, int maxLength, int maxWidth);
};

class Choose123 : public ButtonContainer {
private:
	SpriteResource _iconSprites;

	Choose123(XeenEngine *vm) : ButtonContainer(vm) {}

	int execute(uint numOptions);

	void loadButtons(uint numOptions);
public:
	static int show(XeenEngine *vm, uint numOptions);
};

class HowMuch : public ButtonContainer {
private:
	SpriteResource _iconSprites;

	HowMuch(XeenEngine *vm) : ButtonContainer(vm) {}

	int execute();
public:
	static int show(XeenEngine *vm);
};

} // End of namespace Xeen
} // End of namespace MM

#endif
