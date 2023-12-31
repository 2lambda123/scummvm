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

/*
 * This code is based on the CRAB engine
 *
 * Copyright (c) Arvind Raja Yadav
 *
 * Licensed under MIT
 *
 */

#ifndef CRAB_RADIOBUTTONMENU_H
#define CRAB_RADIOBUTTONMENU_H

#include "crab/ui/menu.h"
#include "crab/ui/RadioButton.h"

namespace Crab {

namespace pyrodactyl {
namespace ui {
class RadioButtonMenu : public Menu<RadioButton> {
	// The description of the menu
	HoverInfo _desc;

	// The selected radio button
	int _select;

public:
	RadioButtonMenu() {
		_select = 0;
	}

	~RadioButtonMenu() {}

	void load(rapidxml::xml_node<char> *node) {
		if (nodeValid("desc", node))
			_desc.load(node->first_node("desc"));

		if (nodeValid("menu", node))
			Menu::load(node->first_node("menu"));
	}

	void draw(const int &xOffset = 0, const int &yOffset = 0) {
		_desc.draw(xOffset, yOffset);
		Menu::draw(xOffset, yOffset);
	}

	int handleEvents(const Common::Event &event, const int &xOffset = 0, const int &yOffset = 0) {
		int result = Menu::handleEvents(event, xOffset, yOffset);

		if (result >= 0) {
			_select = result;

			for (int i = 0; i < (int)_element.size(); ++i)
				_element[i]._state = (i == result);
		}

		return result;
	}

	void setUI() {
		Menu::setUI();
		_desc.setUI();
	}
};
} // End of namespace ui
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_RADIOBUTTONMENU_H
