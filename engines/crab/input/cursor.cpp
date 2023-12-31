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

//=============================================================================
// Author:   Arvind
// Purpose:  Cursor class
//=============================================================================
#include "graphics/cursorman.h"
#include "graphics/managed_surface.h"
#include "crab/crab.h"
#include "crab/input/cursor.h"

namespace Crab {
using namespace pyrodactyl::input;

//------------------------------------------------------------------------
// Purpose: Reset all values
//------------------------------------------------------------------------
void Cursor::reset() {
	_motion.x = 0;
	_motion.y = 0;

	_button.x = 0;
	_button.y = 0;

	_rel.x = 0;
	_rel.y = 0;

	// set to -1, so its set to 0 on first update
	_state = -1;
}

//------------------------------------------------------------------------
// Purpose: Reset all values
//------------------------------------------------------------------------
void Cursor::load(rapidxml::xml_node<char> *node) {
	if (nodeValid("normal", node)) {
		rapidxml::xml_node<char> *nornode = node->first_node("normal");
		_img.load(nornode, "img");
		_imgS.load(nornode, "img_s");
	}

	if (nodeValid("hover", node)) {
		rapidxml::xml_node<char> *hovnode = node->first_node("hover");
		_imgHover.load(hovnode, "img");
		_imgHoverS.load(hovnode, "img_s");

		if (nodeValid("offset", hovnode))
			_hoverOffset.load(hovnode->first_node("offset"));
	}
}

//------------------------------------------------------------------------
// Purpose: Handle Events
//------------------------------------------------------------------------
void Cursor::handleEvents(const Common::Event &event) {
	g_engine->_mouse->_hover = false;

	if (event.type == Common::EVENT_MOUSEMOVE) {
		_motion.x = event.mouse.x;
		_motion.y = event.mouse.y;

		_rel.x = event.relMouse.x;
		_rel.y = event.relMouse.y;
	} else if (event.type == Common::EVENT_LBUTTONDOWN) {
		_pressed = true;

		_button.x = event.mouse.x;
		_button.y = event.mouse.y;
	} else if (event.type == Common::EVENT_LBUTTONUP) {
		_pressed = false;

		_button.x = event.mouse.x;
		_button.y = event.mouse.y;
	}
}

//------------------------------------------------------------------------
// Purpose: Draw
//------------------------------------------------------------------------
void Cursor::draw() {
	uint8 oldState = _state;
	_state = (_hover ? 1 : 0) | (_pressed << 1);

	if (_state != oldState) {
		if (_hover) {
			if (_pressed)
				CursorMan.replaceCursor(_imgHoverS._texture->rawSurface(), 0, 0, 0);
			else
				CursorMan.replaceCursor(_imgHover._texture->rawSurface(), 0, 0, 0);
		} else {
			if (_pressed)
				CursorMan.replaceCursor(_imgS._texture->rawSurface(), 0, 0, 0);
			else
				CursorMan.replaceCursor(_img._texture->rawSurface(), 0, 0, 0);
		}
	}
}

} // End of namespace Crab
