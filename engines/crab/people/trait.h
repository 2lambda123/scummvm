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

#ifndef CRAB_TRAIT_H
#define CRAB_TRAIT_H

#include "crab/image/ImageManager.h"

namespace Crab {

namespace pyrodactyl {
namespace people {
struct Trait {
	// The id of the trait
	int _id;

	// Used for achievements
	Common::String _idStr;

	// The name of the trait
	Common::String _name;

	// The description of the trait
	Common::String _desc;

	// The image used to draw the trait
	ImageKey _img;

	// Indicator for traits the player hasn't read before
	bool _unread;

	Trait() {
		_id = -1;
		_img = 0;
		_unread = true;
	}
	Trait(rapidxml::xml_node<char> *node) : Trait() {
		load(node);
	}

	~Trait() {}

	void clear();
	void load(rapidxml::xml_node<char> *node);
	void saveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root, const char *rootname);
};
} // End of namespace people
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_TRAIT_H
