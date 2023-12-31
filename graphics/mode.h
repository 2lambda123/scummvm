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

#ifndef GRAPHICS_MODE_H
#define GRAPHICS_MODE_H

#include "common/array.h"

namespace Graphics {

/**
 * Represents a hardware video mode.
 */
struct Mode {
	int16 width; ///< The width in pixels
	int16 height; ///< The height in pixels

	Mode(const int16 w, const int16 h) :
		width(w),
		height(h) {}

	bool operator<(const Mode &other) const {
		return width < other.width && height < other.height;
	}
};

typedef Common::Array<Mode> ModeList;

/**
 * Represents a hardware video mode with pixel format.
 */
struct ModeWithFormat {
	int16 width; ///< The width in pixels
	int16 height; ///< The height in pixels
	bool hasFormat; ///< Whether pixel format is valid
	Graphics::PixelFormat format; ///< Pixel format

	ModeWithFormat(const int16 w, const int16 h) :
		width(w), height(h), hasFormat(false) {}
	ModeWithFormat(const int16 w, const int16 h, const Graphics::PixelFormat &f) :
		width(w), height(h), hasFormat(true), format(f) {}
	ModeWithFormat(const int16 w, const int16 h, const Graphics::PixelFormat *f) :
		width(w), height(h), hasFormat(f != nullptr) {
		if (f != nullptr)
			format = *f;
	}
};

typedef Common::Array<ModeWithFormat> ModeWithFormatList;

}

#endif
