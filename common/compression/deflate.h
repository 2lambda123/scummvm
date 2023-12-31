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

#ifndef COMMON_ZLIB_H
#define COMMON_ZLIB_H

#include "common/scummsys.h"
#include "common/types.h"

namespace Common {

/**
 * @defgroup common_deflate deflate
 * @ingroup common
 *
 * @brief API for deflate based operations.
 *
 * @{
 */

class SeekableReadStream;
class WriteStream;

/**
 * Thin wrapper around zlib's uncompress() function. This wrapper makes
 * it possible to uncompress data in engines without being forced to link
 * them against zlib, thus simplifying the build system.
 *
 * Taken from the zlib manual:
 * Decompresses the src buffer into the dst buffer.
 * srcLen is the byte length of the source buffer. Upon entry, dstLen is the
 * total size of the destination buffer, which must be large enough to hold
 * the entire uncompressed data. Upon exit, dstLen is the actual size of the
 * compressed buffer.
 *
 * @param dst       the buffer to store into.
 * @param dstLen    a pointer to the size of the destination buffer.
 * @param src       the data to be decompressed.
 * @param srcLen    the size of the compressed data.
 *
 * @return true on success (i.e. Z_OK), false otherwise.
 */
bool inflateZlib(byte *dst, unsigned long *dstLen, const byte *src, unsigned long srcLen);
static inline bool inflateZlib(byte *dst, unsigned long  dstLen, const byte *src, unsigned long srcLen) {
	return inflateZlib(dst, &dstLen, src, srcLen);
}

/**
 * Wrapper around zlib's inflate functions. This function will call the
 * necessary inflate functions to uncompress data compressed with deflate
 * but *not* with the standard zlib header.
 *
 * Decompresses the src buffer into the dst buffer.
 * srcLen is the byte length of the source buffer, dstLen is the byte
 * length of the output buffer.
 * It decompress as much data as possible, up to dstLen bytes.
 * If a dictionary is provided through the dict buffer, uses it to initializes
 * the internal decompression dictionary, before the decompression takes place.
 *
 * @param dst       the buffer to store into.
 * @param dstLen    the size of the destination buffer.
 * @param src       the data to be decompressed.
 * @param srcLen    the size of the compressed data.
 * @param dict      (optional) a decompress dictionary.
 * @param dictLen   (optional) the size of the dictionary.
 *                  Mandatory if dict is not 0.
 *
 * @return true on success (Z_OK or Z_STREAM_END), false otherwise.
 */
bool inflateZlibHeaderless(byte *dst, uint *dstLen, const byte *src, uint srcLen, const byte *dict = nullptr, uint dictLen = 0);
static inline bool inflateZlibHeaderless(byte *dst, uint dstLen, const byte *src, uint srcLen, const byte *dict = nullptr, uint dictLen = 0) {
	return inflateZlibHeaderless(dst, &dstLen, src, srcLen, dict, dictLen);
}

/**
 * Wrapper around zlib's inflate functions. This function will call the
 * modified inflate functions to uncompress data compressed for Clickteam
 * Installer files.
 *
 * Decompresses the src buffer into the dst buffer.
 * srcLen is the byte length of the source buffer, dstLen is the byte
 * length of the output buffer.
 * It decompress as much data as possible, up to dstLen bytes.
 *
 * @param dst       the buffer to store into.
 * @param dstLen    the size of the destination buffer.
 * @param src       the data to be decompressed.
 * @param srcLen    the size of the compressed data.
 *
 * @return true on success (Z_OK or Z_STREAM_END), false otherwise.
 */
bool inflateClickteam(byte *dst, uint *dstLen, const byte *src, uint srcLen);
static inline bool inflateClickteam(byte *dst, uint  dstLen, const byte *src, uint srcLen) {
	return inflateClickteam(dst, &dstLen, src, srcLen);
}

/**
 * Take an arbitrary SeekableReadStream and wrap it in a custom stream which
 * provides transparent on-the-fly decompression. Assumes the data it
 * retrieves from the wrapped stream to be either uncompressed or in gzip
 * format. In the former case, the original stream is returned unmodified
 * (and in particular, not wrapped). In the latter case the stream is
 * returned wrapped, unless there is no ZLIB support, then NULL is returned
 * and the old stream is destroyed.
 *
 * Certain GZip-formats don't supply an easily readable length, if you
 * still need the length carried along with the stream, and you know
 * the decompressed length at wrap-time, then it can be supplied as knownSize
 * here. knownSize will be ignored if the GZip-stream DOES include a length.
 * The created stream also becomes responsible for freeing the passed stream.
 *
 * It is safe to call this with a NULL parameter (in this case, NULL is
 * returned).
 *
 * @param toBeWrapped	the stream to be wrapped (if it is in gzip-format)
 * @param knownSize	a supplied length of the uncompressed data (if not available directly)
 */
SeekableReadStream *wrapCompressedReadStream(SeekableReadStream *toBeWrapped,
		DisposeAfterUse::Flag disposeParent = DisposeAfterUse::YES, uint64 knownSize = 0);

/**
 * Take an arbitrary SeekableReadStream and wrap it in a custom stream which
 * provides transparent on-the-fly decompression. Assumes the data it
 * retrieves from the wrapped stream is compressed with deflate algorithm.
 *
 * It is safe to call this with a NULL parameter (in this case, NULL is
 * returned).
 *
 * @param toBeWrapped	the stream to be wrapped (if it is in gzip-format)
 * @param knownSize	a supplied length of the uncompressed data (if not available directly)
 */
SeekableReadStream *wrapDeflateReadStream(SeekableReadStream *toBeWrapped,
		DisposeAfterUse::Flag disposeParent = DisposeAfterUse::YES, uint64 knownSize = 0,
		const byte *dict = nullptr, uint dictLen = 0);

/**
 * Take an arbitrary SeekableReadStream and wrap it in a custom stream which
 * provides transparent on-the-fly decompression. Assumes the data it
 * retrieves from the wrapped stream is compressed with the Clickteam deflate
 * variant algorithm.
 * The stream is returned wrapped, unless there is no ZLIB support,
 * then NULL is returned and the old stream is destroyed.
 *
 * It is safe to call this with a NULL parameter (in this case, NULL is
 * returned).
 *
 * @param toBeWrapped	the stream to be wrapped (if it is in gzip-format)
 * @param knownSize	a supplied length of the compressed data (if not available directly)
 */
SeekableReadStream *wrapClickteamReadStream(SeekableReadStream *toBeWrapped,
		DisposeAfterUse::Flag disposeParent = DisposeAfterUse::YES, uint64 knownSize = 0);

/**
 * Take an arbitrary WriteStream and wrap it in a custom stream which provides
 * transparent on-the-fly compression. The compressed data is written in the
 * gzip format, unless ZLIB support has been disabled, in which case the given
 * stream is returned unmodified (and in particular, not wrapped).
 * The created stream also becomes responsible for freeing the passed stream.
 *
 * It is safe to call this with a NULL parameter (in this case, NULL is
 * returned).
 */
WriteStream *wrapCompressedWriteStream(WriteStream *toBeWrapped);

/** @} */

} // End of namespace Common

#endif
