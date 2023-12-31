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

#ifndef XEEN_DIALOGS_PARTY_H
#define XEEN_DIALOGS_PARTY_H

#include "common/array.h"
#include "mm/xeen/dialogs/dialogs.h"
#include "mm/xeen/interface.h"
#include "mm/xeen/screen.h"
#include "mm/xeen/sprites.h"

namespace MM {
namespace Xeen {

/**
 * Shows the Party dialog that's shown when signing into an inn
 */
class PartyDialog : public ButtonContainer, public PartyDrawer {
private:
	XeenEngine *_vm;
	SpriteResource _uiSprites;
	DrawStruct _faceDrawStructs[4];
	Common::String _partyDetails;
	Common::Array<int> _charList;

	/**
	 * Constructor
	 */
	PartyDialog(XeenEngine *vm);

	/**
	 * Executes the dialog
	 */
	void execute();

	/**
	 * Loads the list of character
	 */
	void loadCharacters();

	/**
	 * Loads buttons for the dialog
	 */
	void loadButtons();

	/**
	 * Initialises a list of elements to draw
	 */
	void initDrawStructs();

	/**
	 * Sets up the background
	 */
	void setupBackground();

	/**
	 * Sets up the faces from the avaialble roster for display in the party dialog
	 */
	void setupFaces(int firstDisplayChar, bool updateFlag);

	void startingCharChanged(int firstDisplayChar);

	int selectCharacter(bool isDelete, int firstDisplayChar);
public:
	/**
	 * Show the Party dialog
	 */
	static void show(XeenEngine *vm);
};

} // End of namespace Xeen
} // End of namespace MM

#endif
