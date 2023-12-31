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
 * To ensure exclusive use of resources and exclusive control responsibilities.
 */

#include "common/util.h"

#include "tinsel/sched.h"
#include "tinsel/token.h"

namespace Tinsel {

//----------------- LOCAL GLOBAL DATA --------------------

struct Token {
	Common::PROCESS		*proc;
	bool				isFree;
};

// These vars are reset upon engine destruction
static Token g_tokens[NUMTOKENS];


/**
 * Release all tokens held by this process, and kill the process.
 */
static void TerminateProcess(Common::PROCESS *tProc) {

	// Release tokens held by the process
	for (int i = 0; i < NUMTOKENS; i++) {
		if (!g_tokens[i].isFree && g_tokens[i].proc == tProc) {
			g_tokens[i].proc = nullptr;
			g_tokens[i].isFree = true;
		}
	}

	// Kill the process
	if (tProc != nullptr)
		CoroScheduler.killProcess(tProc);
}

/**
 * Gain control of the CONTROL token if it is free.
 */
void GetControlToken() {
	const int which = TOKEN_CONTROL;

	if (g_tokens[which].isFree) {
		g_tokens[which].proc = CoroScheduler.getCurrentProcess();
		g_tokens[which].isFree = false;
	}
}

/**
 * Release control of the CONTROL token.
 */
void FreeControlToken() {
	// Allow anyone to free TOKEN_CONTROL
	g_tokens[TOKEN_CONTROL].proc = nullptr;
	g_tokens[TOKEN_CONTROL].isFree = true;
}


/**
 * Gain control of a token. If the requested token is out of range, or
 * is already held by the calling process, then the calling process
 * will be killed off.
 *
 * Otherwise, the calling process will gain the token. If the token was
 * held by another process, then the previous holder is killed off.
 */
void GetToken(int which) {
	assert(TOKEN_LEAD <= which && which < NUMTOKENS);

	if (!g_tokens[which].isFree) {
		assert(g_tokens[which].proc != CoroScheduler.getCurrentProcess());
		TerminateProcess(g_tokens[which].proc);
	}

	g_tokens[which].proc = CoroScheduler.getCurrentProcess();
	g_tokens[which].isFree = false;
}

/**
 * Release control of a token. If the requested token is not owned by
 * the calling process, then the calling process will be killed off.
 */
void FreeToken(int which) {
	assert(TOKEN_LEAD <= which && which < NUMTOKENS);

	assert(g_tokens[which].proc == CoroScheduler.getCurrentProcess());	// we'd have been killed if some other proc had taken this token

	g_tokens[which].proc = nullptr;
	g_tokens[which].isFree = true;
}

/**
 * If it's a valid token and it's free, returns true.
 */
bool TestToken(int which) {
	if (which < 0 || which >= NUMTOKENS)
		return false;

	return (g_tokens[which].isFree);
}

/**
 * Call at the start of each scene.
 */
void FreeAllTokens() {
	for (int i = 0; i < NUMTOKENS; i++) {
		g_tokens[i].proc = nullptr;
		g_tokens[i].isFree = true;
	}
}

} // End of namespace Tinsel
