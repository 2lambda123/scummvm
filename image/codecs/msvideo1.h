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

#ifndef IMAGE_CODECS_MSVIDEO1_H
#define IMAGE_CODECS_MSVIDEO1_H

#include "image/codecs/codec.h"

namespace Image {

/**
 * Microsoft Video 1 decoder.
 *
 * Used by BMP/AVI.
 */
class MSVideo1Decoder : public Codec {
public:
	MSVideo1Decoder(uint16 width, uint16 height, byte bitsPerPixel);
	~MSVideo1Decoder() override;

	const Graphics::Surface *decodeFrame(Common::SeekableReadStream &stream) override;
	Graphics::PixelFormat getPixelFormat() const override { return _surface->format; }

private:
	byte _bitsPerPixel;

	Graphics::Surface *_surface;

	void decode8(Common::SeekableReadStream &stream);
	void decode16(Common::SeekableReadStream &stream);
};

} // End of namespace Image

#endif
