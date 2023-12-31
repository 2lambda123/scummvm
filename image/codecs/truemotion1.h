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

// Based on the TrueMotion 1 decoder by Alex Beregszaszi & Mike Melanson in FFmpeg

// Only compile if SCI32 is enabled, TESTBED is enabled, ZVISION is enabled, or if we're building dynamic modules
#if defined(ENABLE_SCI32) || defined(ENABLE_TESTBED) || defined(ENABLE_ZVISION) || defined(DYNAMIC_MODULES)

#ifndef IMAGE_CODECS_TRUEMOTION1_H
#define IMAGE_CODECS_TRUEMOTION1_H

#include "image/codecs/codec.h"

namespace Image {

/**
 * Duck TrueMotion 1 decoder.
 *
 * Used by BMP/AVI.
 */
class TrueMotion1Decoder : public Codec {
public:
	TrueMotion1Decoder();
	~TrueMotion1Decoder() override;

	const Graphics::Surface *decodeFrame(Common::SeekableReadStream &stream) override;

	// Always return RGB565
	Graphics::PixelFormat getPixelFormat() const override { return Graphics::PixelFormat(2, 5, 6, 5, 0, 11, 5, 0, 0); }

private:
	Graphics::Surface *_surface;

	int _mbChangeBitsRowSize;
	byte *_buf, *_mbChangeBits, *_indexStream;
	int _indexStreamSize;

	int _flags;

	struct PredictorTableEntry {
		uint32 color;
		bool getNextIndex;
	};

	PredictorTableEntry _yPredictorTable[1024];
	PredictorTableEntry _cPredictorTable[1024];

	int _blockType;
	int _blockWidth;
	int _blockHeight;

	int16 _ydt[8];
	int16 _cdt[8];

	int _lastDeltaset, _lastVectable;

	uint32 *_vertPred;

	struct {
		byte headerSize;
		byte compression;
		byte deltaset;
		byte vectable;
		uint16 ysize;
		uint16 xsize;
		uint16 checksum;
		byte version;
		byte headerType;
		byte flags;
		byte control;
		uint16 xoffset;
		uint16 yoffset;
		uint16 width;
		uint16 height;
	} _header;

	void selectDeltaTables(int deltaTableIndex);
	void decodeHeader(Common::SeekableReadStream &stream);
	void decode16();
	int makeYdt16Entry(int p1, int p2);
	int makeCdt16Entry(int p1, int p2);
	void genVectorTable16(const byte *selVectorTable);
};

} // End of namespace Image

#endif // IMAGE_CODECS_TRUEMOTION1_H
#endif // SCI32/Plugins guard
