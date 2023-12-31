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

#ifndef CRAB_RADIOBUTTON_H
#define CRAB_RADIOBUTTON_H

#include "crab/ui/ToggleButton.h"

namespace Crab {

namespace pyrodactyl {
namespace ui {
class RadioButton : public ToggleButton {
public:
	// The value associated with the radio button
	float _val;

	RadioButton() {
		_val = 0.0f;
	}
	
	~RadioButton() {}

	void load(rapidxml::xml_node<char> *node) {
		ToggleButton::load(node);
		loadNum(_val, "val", node);
	}
};
} // End of namespace ui
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_RADIOBUTTON_H
