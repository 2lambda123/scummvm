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

#ifndef CRAB_IMAGEEFFECT_H
#define CRAB_IMAGEEFFECT_H

#include "crab/image/ImageManager.h"

namespace Crab {

namespace pyrodactyl {
namespace anim {
struct ImageEffect : public Vector2i {
	ImageKey _img;
	bool _visible;

	ImageEffect() {
		_visible = false;
		_img = 0;
	}

	void load(rapidxml::xml_node<char> *node);
	void draw(const int &xOffset, const int &yOffset);
};
} // End of namespace anim
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_IMAGEEFFECT_H
