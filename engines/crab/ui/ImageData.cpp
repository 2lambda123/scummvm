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

#include "crab/crab.h"
#include "crab/image/ImageManager.h"
#include "crab/ui/ImageData.h"

namespace Crab {

using namespace pyrodactyl::ui;
using namespace pyrodactyl::image;

void ImageData::load(rapidxml::xml_node<char> *node, const bool &echo) {
	loadImgKey(_key, "img", node, echo);
	loadBool(_crop, "crop", node, false);

	if (nodeValid("clip", node, false))
		_clip.load(node->first_node("clip"));

	Element::load(node, _key, echo);
}

void ImageData::draw(const int &xOffset, const int &yOffset) {
	if (_crop)
		g_engine->_imageManager->draw(x + xOffset, y + yOffset, _key, &_clip);
	else
		g_engine->_imageManager->draw(x + xOffset, y + yOffset, _key);
}

} // End of namespace Crab
