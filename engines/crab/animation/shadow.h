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

#ifndef CRAB_SHADOW_H
#define CRAB_SHADOW_H

#include "crab/crab.h"
#include "crab/vectors.h"
#include "crab/image/ImageManager.h"

namespace Crab {

namespace pyrodactyl {
namespace anim {
struct ShadowData {
	// The image of the sprite's shadow
	ImageKey _img;

	// Size of image
	Vector2i _size;

	// The default shadow offset
	Vector2i _offset;

	ShadowData() : _size(1, 1) {
		_img = 0;
	}

	void load(rapidxml::xml_node<char> *node) {
		loadImgKey(_img, "img", node);
		_offset.load(node);

		using namespace pyrodactyl::image;
		Image dat;
		g_engine->_imageManager->getTexture(_img, dat);
		_size.x = dat.w() / 2;
		_size.y = dat.h() / 2;
	}
};

// Used when a set of animation frames needs a specific shadow offset
class ShadowOffset : public Vector2i {
public:
	// Only use this offset if this is true
	bool _valid;

	ShadowOffset() {
		_valid = false;
	}
};
} // End of namespace anim
} // End of namespace pyrodactyl

} // End of namespace Crab

#endif // CRAB_SHADOW_H
