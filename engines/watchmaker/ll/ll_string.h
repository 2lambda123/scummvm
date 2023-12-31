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

#ifndef WATCHMAKER_LL_STRING_H
#define WATCHMAKER_LL_STRING_H

#include "watchmaker/types.h"
#include "watchmaker/globvar.h"

namespace Watchmaker {

class WGame;

void ClearText();
void Text(uint16 x, uint16 y, uint16 dx, char *text);
class Fonts;
uint16 TextLen(Fonts &fonts, char *sign, uint16 num);
void PaintInventory(WGame &game);
void PaintText(WGame &game);

} // End of namespace Watchmaker

#endif // WATCHMAKER_LL_STRING_H
