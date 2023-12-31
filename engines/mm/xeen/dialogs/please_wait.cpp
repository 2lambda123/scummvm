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

#include "mm/xeen/dialogs/please_wait.h"
#include "mm/xeen/resources.h"
#include "mm/xeen/window.h"
#include "mm/xeen/xeen.h"

namespace MM {
namespace Xeen {

PleaseWait::PleaseWait(bool isOops) {
	_msg = isOops ? Res.OOPS : Res.PLEASE_WAIT;
}

PleaseWait::~PleaseWait() {
	Windows &windows = *g_vm->_windows;
	windows[9].close();
}

void PleaseWait::show() {
	if (g_vm->_mode == MODE_STARTUP) {
		return;
	}

	Windows &windows = *g_vm->_windows;
	Window &w = windows[9];
	w.open();
	w.writeString(_msg);
	w.update();
}

} // End of namespace Xeen
} // End of namespace MM
