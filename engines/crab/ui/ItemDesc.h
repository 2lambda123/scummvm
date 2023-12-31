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

#ifndef CRAB_ITEMDESC_H
#define CRAB_ITEMDESC_H

#include "crab/ui/ParagraphData.h"

namespace Crab {

namespace pyrodactyl {
namespace ui {
class ItemDesc {
	// The name of the item
	TextData _name;

	// The description of the item
	ParagraphData _desc;

public:
	ItemDesc() {}
	~ItemDesc() {}

	void load(rapidxml::xml_node<char> *node) {
		if (nodeValid("name", node))
			_name.load(node->first_node("name"));

		if (nodeValid("desc", node))
			_desc.load(node->first_node("desc"));
	}

	void draw(pyrodactyl::item::Item &item) {
		_name.draw(item._name);
		_desc.draw(item._desc);
	}

	void setUI() {
		_name.setUI();
		_desc.setUI();
	}
};
} // End of namespace ui
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_ITEMDESC_H
