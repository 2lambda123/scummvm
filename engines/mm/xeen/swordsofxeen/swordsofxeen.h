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

#ifndef XEEN_SWORDSOFXEEN_SWORDSOFXEEN_H
#define XEEN_SWORDSOFXEEN_SWORDSOFXEEN_H

#include "mm/xeen/xeen.h"
#include "mm/xeen/worldofxeen/worldofxeen_cutscenes.h"

namespace MM {
namespace Xeen {
namespace SwordsOfXeen {

/**
 * Implements a descendant of the base Xeen engine to handle
 * Swords of Xeen specific game code
 */
class SwordsOfXeenEngine: public XeenEngine {
private:
	/**
	 * Show the ending "You won" screen
	 */
	bool showEnding();
protected:
	/**
	 * Show the starting sequence/intro
	 */
	void showStartup() override {
		// Swords of Xeen doesn't have a starting title or intro
		_gameMode = GMODE_MENU;
	}

	/**
	 * Show the startup menu
	 */
	void showMainMenu() override;

	/**
	 * Death cutscene
	 */
	void death() override;
public:
	SwordsOfXeenEngine(OSystem *syst, const MightAndMagicGameDescription *gameDesc);
	~SwordsOfXeenEngine() override {}

	/**
	 * Show a cutscene
	 */
	void showCutscene(const Common::String &name, int status, uint score) override;

	/**
	 * Dream sequence
	 */
	void dream() override;
};

#define SWORDS_VM (*(::Xeen::SwordsOfXeen::SwordsOfXeenEngine *)g_vm)

} // End of namespace SwordsOfXeen
} // End of namespace Xeen
} // End of namespace MM

#endif
