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
#include "crab/level/level_objects.h"

namespace Crab {

using namespace pyrodactyl::image;
using namespace pyrodactyl::level;

void PlayerDestMarker::load(rapidxml::xml_node<char> *node) {
	loadImgKey(_img, "dest", node);
	_size.x = g_engine->_imageManager->getTexture(_img).w();
	_size.y = g_engine->_imageManager->getTexture(_img).h();
}

void PlayerDestMarker::draw(const Vector2i &pos, const Rect &camera) {
	g_engine->_imageManager->draw(pos.x - (_size.x / 2) - camera.x, pos.y - (_size.y / 2) - camera.y, _img);
}

} // End of namespace Crab
