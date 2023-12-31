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

#include "mm/xeen/dialogs/dialogs_quick_fight.h"
#include "mm/xeen/resources.h"
#include "mm/xeen/xeen.h"

namespace MM {
namespace Xeen {

void QuickFight::show(XeenEngine *vm, Character *currentChar) {
	QuickFight *dlg = new QuickFight(vm, currentChar);
	dlg->execute();
	delete dlg;
}

QuickFight::QuickFight(XeenEngine *vm, Character *currentChar) : ButtonContainer(vm),
		_currentChar(currentChar) {
	loadButtons();
}

void QuickFight::execute() {
	Combat &combat = *_vm->_combat;
	EventsManager &events = *_vm->_events;
	Interface &intf = *_vm->_interface;
	Party &party = *_vm->_party;
	Windows &windows = *_vm->_windows;
	Window &w = windows[10];
	w.open();

	do {
		// Draw the dialog text and buttons
		Common::String msg = Common::String::format(Res.QUICK_FIGHT_TEXT,
			_currentChar->_name.c_str(),
			Res.QUICK_FIGHT_OPTIONS[_currentChar->_quickOption]);
		w.writeString(msg);
		drawButtons(&w);

		// Wait for selection
		_buttonValue = 0;
		events.updateGameCounter();
		do {
			intf.draw3d(false, false);

			events.pollEventsAndWait();
			checkEvents(_vm);
			if (_vm->shouldExit())
				return;
		} while (!_buttonValue && !events.timeElapsed());

		if (Common::KEYCODE_F1 == _buttonValue || 
			Common::KEYCODE_F2 == _buttonValue || 
			Common::KEYCODE_F3 == _buttonValue || 
			Common::KEYCODE_F4 == _buttonValue || 
			Common::KEYCODE_F5 == _buttonValue || 
			Common::KEYCODE_F6 == _buttonValue) {
			int charIdx = _buttonValue - Common::KEYCODE_F1;
			if (charIdx < (int)combat._combatParty.size()) {
				// Highlight new character
				_currentChar = &party._activeParty[charIdx];
				intf.highlightChar(charIdx);
			}
		} else if (Common::KEYCODE_n == _buttonValue || 
				   Res.KeyConstants.DialogsQuickFight.KEY_NEXT == _buttonValue) {
			_currentChar->_quickOption = (QuickAction)(((int)_currentChar->_quickOption + 1) % 4);
		}
	} while (_buttonValue != Common::KEYCODE_RETURN && _buttonValue != Common::KEYCODE_ESCAPE);

	w.close();
	events.clearEvents();
}

void QuickFight::loadButtons() {
	_icons.load("train.icn");
	addButton(Common::Rect(281, 108, 305, 128), Common::KEYCODE_ESCAPE, &_icons);

	addButton(Common::Rect(242, 108, 266, 128), Res.KeyConstants.DialogsQuickFight.KEY_NEXT, &_icons);
}

} // End of namespace Xeen
} // End of namespace MM
