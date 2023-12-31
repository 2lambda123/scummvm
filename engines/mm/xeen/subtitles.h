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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef XEEN_SUBTITLES_H
#define XEEN_SUBTITLES_H

#include "common/str-array.h"
#include "mm/xeen/sprites.h"

namespace MM {
namespace Xeen {

class Subtitles {
private:
	Common::StringArray _lines;
	int _lineNum;
	SpriteResource *_boxSprites;
	int _lineEnd, _lineSize;
	Common::String _displayLine;
private:
	/**
	 * Loads the string list of all subtitles
	 */
	void loadSubtitles();

	/**
	 * Mark the current time
	 */
	void markTime();

	/**
	 * Returns true if the time for a subtitle frame has expired
	 */
	bool timeElapsed() const;
public:
	/**
	 * Constructor
	 */
	Subtitles();

	/**
	 * Destructor
	 */
	virtual ~Subtitles();

	/**
	 * Set which subtitle line to display
	 */
	void setLine(int line);

	/**
	 * Resets subtitles, stopping any display
	 */
	void reset();

	/**
	 * Returns true if a subtitle is active
	 */
	bool active() const;

	/**
	 * Returns true if a subtitle is active or a voice line is currently being played
	 */
	bool lineActive() const;

	/**
	 * Shows any active subtitle
	 */
	void show();

	/**
	 * Waits for a given number of frames
	 * @param numFrames			Number of frames to wait
	 * @param interruptable		If set, aborts if the mouse or a key is pressed
	 * @returns		True if the wait was aborted
	 */
	bool wait(uint numFrames, bool interruptable = true);

	/**
	 * Wait for the end of currently playing sound or subtitles line
	 */
	bool waitForLineOrSound();
};

} // End of namespace Xeen
} // End of namespace MM

#endif
