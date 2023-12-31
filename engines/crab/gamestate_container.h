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
// Purpose:  The game state virtual class
//=============================================================================
#ifndef CRAB_GAMESTATE_CONTAINER_H
#define CRAB_GAMESTATE_CONTAINER_H

#include "common/events.h"
#include "crab/gamestates.h"

namespace Crab {

//------------------------------------------------------------------------
// Purpose: Game State base class
//------------------------------------------------------------------------
class GameState {
public:
	virtual void handleEvents(Common::Event &event, bool &shouldChangeState, GameStateID &newStateId) = 0;
	virtual void internalEvents(bool &shouldChangeState, GameStateID &newStateId) = 0;
	virtual void draw() = 0;
	virtual void setUI() = 0;
	virtual void autoSave() = 0;
	virtual ~GameState(){};
};

} // End of namespace Crab

#endif // CRAB_GAMESTATE_CONTAINER_H
