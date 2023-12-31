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

#ifndef CRAB_RANGE_H
#define CRAB_RANGE_H

#include "crab/animation/animframe.h"

namespace Crab {

namespace pyrodactyl {
namespace anim {
struct Range {
	bool _valid;
	Rect _val[DIRECTION_TOTAL];

	Range() {
		_valid = false;
	}

	void load(rapidxml::xml_node<char> *node) {
		if (nodeValid(node, false)) {
			_val[DIRECTION_DOWN].load(node->first_node("down"));
			_val[DIRECTION_UP].load(node->first_node("up"));
			_val[DIRECTION_LEFT].load(node->first_node("left"));
			_val[DIRECTION_RIGHT].load(node->first_node("right"));

			_valid = true;
		} else
			_valid = false;
	}
};
} // End of namespace anim
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_RANGE_H
