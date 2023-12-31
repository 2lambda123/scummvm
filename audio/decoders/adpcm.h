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

/**
 * @file
 * Sound decoder used in engines:
 *  - agos
 *  - lastexpress
 *  - mohawk
 *  - saga
 *  - sci (DK3 ADPCM for Phantasmagoria 2)
 *  - scumm
 *  - tinsel
 */

#ifndef AUDIO_ADPCM_H
#define AUDIO_ADPCM_H

#include "common/scummsys.h"
#include "common/types.h"

namespace Common {
class SeekableReadStream;
}


namespace Audio {

class PacketizedAudioStream;
class SeekableAudioStream;

// There are several types of ADPCM encoding, only some are supported here
// For all the different encodings, refer to:
// http://wiki.multimedia.cx/index.php?title=Category:ADPCM_Audio_Codecs
// Usually, if the audio stream we're trying to play has the FourCC header
// string intact, it's easy to discern which encoding is used
enum ADPCMType {
	kADPCMOki,                 // Dialogic/Oki ADPCM (aka VOX)
	kADPCMMSIma,               // Microsoft IMA ADPCM
	kADPCMMS,                  // Microsoft ADPCM
	kADPCMDVI,                 // Intel DVI IMA ADPCM
	kADPCMApple,               // Apple QuickTime IMA ADPCM
	kADPCMDK3,                 // Duck DK3 IMA ADPCM
	kADPCMXA                   // XA ADPCM
};

/**
 * Takes an input stream containing ADPCM compressed sound data and creates
 * a RewindableAudioStream from that.
 *
 * @param stream            the SeekableReadStream from which to read the ADPCM data
 * @param disposeAfterUse   whether to delete the stream after use
 * @param size              how many bytes to read from the stream (0 = all)
 * @param type              the compression type used
 * @param rate              the sampling rate
 * @param channels          the number of channels
 * @param blockAlign        block alignment ???
 * @return   a new RewindableAudioStream, or NULL, if an error occurred
 */
SeekableAudioStream *makeADPCMStream(
	Common::SeekableReadStream *stream,
	DisposeAfterUse::Flag disposeAfterUse,
	uint32 size, ADPCMType type,
	int rate,
	int channels,
	uint32 blockAlign = 0);

/**
 * Creates a PacketizedAudioStream that will automatically queue
 * packets as individual AudioStreams like returned by makeADPCMStream.
 *
 * Due to the ADPCM types not necessarily supporting stateless
 * streaming, OKI, XA and DVI are not supported by this function
 * and will return NULL.
 *
 * @param type              the compression type used
 * @param rate              the sampling rate
 * @param channels          the number of channels
 * @param blockAlign        block alignment ???
 * @return The new PacketizedAudioStream or NULL, if the type isn't supported.
 */
PacketizedAudioStream *makePacketizedADPCMStream(
	ADPCMType type,
	int rate,
	int channels,
	uint32 blockAlign = 0);

} // End of namespace Audio

#endif
