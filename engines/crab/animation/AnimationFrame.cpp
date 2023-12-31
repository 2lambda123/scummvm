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
#include "crab/animation/AnimationFrame.h"

namespace Crab {

using namespace pyrodactyl::image;
using namespace pyrodactyl::anim;

AnimationFrame::AnimationFrame(rapidxml::xml_node<char> *node) : AnimationFrame() {
	_eff = AnimationEffect(node);
	Vector2i::load(node);
	loadImgKey(_img, "img", node);
	loadNum(_start, "start", node);
	loadNum(_finish, "finish", node);
	loadColor(_col, node);

	if (nodeValid("text", node, false))
		_text.load(node->first_node("text"));

	reset();
}

void AnimationFrame::reset() {
	switch (_eff._type) {
	case FADE_IN:
		_col.a = 0;
		break;
	case FADE_OUT:
		_col.a = 255;
		break;
	default:
		_col.a = 255;
		break;
	}
}

void AnimationFrame::draw(const uint32 &timestamp) {
	warning("STUB: AnimationFrame::draw()");

#if 0
	// Only draw the frame in the specified duration
	if (timestamp >= start && timestamp <= finish) {
		// Fill the screen with the color indicated
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(gRenderer, col.r, col.g, col.b, col.a);
		SDL_RenderFillRect(gRenderer, NULL);

		g_engine->_imageManager->draw(x, y, img);
		text.draw();
	}
#endif
}

DrawType AnimationFrame::internalEvents(const uint32 &timestamp) {

	// Vary alpha according to the effect values in the variation time frame
	if (timestamp >= _eff._start && timestamp <= _eff._finish) {
		// These equations courtesy of linear algebra
		switch (_eff._type) {
		case FADE_IN:
			_col.a = (255 * (timestamp - _eff._start)) / (_eff._finish - _eff._start);
			break;
		case FADE_OUT:
			_col.a = (255 * (_eff._finish - timestamp)) / (_eff._finish - _eff._start);
			break;
		default:
			break;
		}

		return _eff._drawGame;
	}

	return DRAW_SAME;
}

} // End of namespace Crab
