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

#ifndef MM1_VIEWS_TEXT_VIEW_H
#define MM1_VIEWS_TEXT_VIEW_H

#include "mm/mm1/events.h"

namespace MM {
namespace MM1 {
namespace Views {

#define FONT_SIZE 8
#define TEXT_W 40
#define TEXT_H 25

class TextView : public UIElement {
protected:
	Common::Point _textPos;

	/**
	 * Write a character
	 */
	void writeChar(char c);
	void writeChar(int x, int y, char c);

	/**
	 * Write some text
	 */
	void writeString(const Common::String &str);
	void writeString(int x, int y, const Common::String &str);

	/**
	 * Write a number
	 */
	void writeNumber(int val);
	void writeNumber(int x, int y, int val);

	/**
	 * Move the text position to the next line
	 */
	void newLine();

	/**
	 * Clear the surface
	 */
	void clearSurface() override;

	/**
	 * Clear area covered by the given text lines
	 */
	void clearLines(int y1, int y2);

	/**
	 * Draws a text border
	 */
	void drawTextBorder();

	/**
	 * Prints a message 'ESC' to go back
	 */
	void escToGoBack();
public:
	TextView(const Common::String &name);
	TextView(const Common::String &name, UIElement *owner);
	virtual ~TextView() {}
};

} // namespace Views
} // namespace MM1
} // namespace MM

#endif