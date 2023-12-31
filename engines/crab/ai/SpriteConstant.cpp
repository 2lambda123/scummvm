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

#include "crab/ai/SpriteConstant.h"

namespace Crab {

using namespace pyrodactyl::ai;

FlyerConstant::FlyerConstant() : _start(10, 40), _vel(8.0f, 0.0f) {
	_delayMin = 5000;
	_delayMax = 20000;
}

void FlyerConstant::load(rapidxml::xml_node<char> *node) {
	if (nodeValid("start", node))
		_start.load(node->first_node("start"));

	if (nodeValid("vel", node))
		_vel.load(node->first_node("vel"));

	if (nodeValid("delay", node)) {
		auto n = node->first_node("delay");
		loadNum(_delayMin, "min", n);
		loadNum(_delayMax, "max", n);
	}
}

SpriteConstant::SpriteConstant() : _walkVelMod(0.9f, 0.63f) {
	_planeW = 20;
	_tweening = 0.2f;
}

void SpriteConstant::load(rapidxml::xml_node<char> *node) {
	loadNum(_planeW, "plane_width", node);
	loadNum(_tweening, "tweening", node);

	if (nodeValid("_walkVelMod", node))
		_walkVelMod.load(node->first_node("_walkVelMod"));

	if (nodeValid("fly", node))
		_fly.load(node->first_node("fly"));
}

} // End of namespace Crab
