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

#ifndef CRAB_LEVELEXIT_H
#define CRAB_LEVELEXIT_H

#include "crab/Shape.h"

namespace Crab {

namespace pyrodactyl {
namespace level {
struct Exit {
	Common::String _name;
	Shape _dim;
	Vector2i _entry;

	Exit() : _entry(-1, -1) {}
	Exit(rapidxml::xml_node<char> *node) {
		load(node);
	}

	void load(rapidxml::xml_node<char> *node);
};
} // End of namespace level
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_LEVELEXIT_H
