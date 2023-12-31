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

#ifndef XEEN_DIALOGS_CREATE_CHAR_H
#define XEEN_DIALOGS_CREATE_CHAR_H

#include "mm/xeen/dialogs/dialogs.h"
#include "mm/xeen/character.h"

namespace MM {
namespace Xeen {

class CreateCharacterDialog : public ButtonContainer  {
private:
	SpriteResource _icons;
	SpriteResource _dice;
	Common::Point _diceSize;
	int _diceFrame[3];
	Common::Point _dicePos[3];
	Common::Point _diceInc[3];
	uint _attribs[TOTAL_ATTRIBUTES];
	bool _allowedClasses[TOTAL_CLASSES];
private:
	/**
	 * Constructor
	 */
	CreateCharacterDialog(XeenEngine *vm);

	/**
	 * Loads the buttons for the dialog
	 */
	void loadButtons();

	/**
	 * Draws on-screen icons
	 */
	void drawIcons();

	/**
	 * Draws on-screen icons
	 */
	void drawIcons2();

	/**
	 * Animate the dice rolling around
	 */
	void drawDice();

	/**
	 * Executes the dialog
	 */
	void execute();

	/**
	 * Returns the attribute that a given keycode represents
	 */
	int getAttribFromKeycode(int keycode) const;

	/**
	 * Handles the logic for swapping attributes
	 * @param keycode		Key pressed representing one of the attributes
	 * @returns		True if swap occurred
	 */
	bool swapAttributes(int keycode);

	/**
	 * Exchanging two attributes for the character being rolled
	 */
	int exchangeAttribute(int srcAttr);

	/**
	 * Set a list of flags for which classes the passed attribute set meet the
	 * minimum requirements of
	 */
	void checkClass();

	/**
	 * Return details of the generated character
	 */
	int newCharDetails(Race race, Sex sex, int classId, int selectedClass, Common::String &msg);

	/**
	 * Print the selection arrow to indicate the selected class
	 */
	void printSelectionArrow(int selectedClass);

	/**
	 * Saves the rolled character into the roster
	 */
	bool saveCharacter(Character &c, int classId, Race race, Sex sex);

	/**
	 * Roll up some random values for the attributes, and return both them as
	 * well as a list of classes that the attributes meet the requirements for
	 */
	void rollAttributes();
public:
	/**
	 * Shows the Create Character dialog
	 */
	static void show(XeenEngine *vm);
};

} // End of namespace Xeen
} // End of namespace MM

#endif
