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

#ifndef CRAB_PATHFINDINGGRID_H
#define CRAB_PATHFINDINGGRID_H

#include "crab/PathfindingGraphNode.h"

namespace Crab {

namespace TMX {
class TMXMap;
}

// This is the grid of pathfinding nodes that is formed for the level (SZ)
class PathfindingGrid {
	friend class PathfindingGraphNode;

	PathfindingGraphNode **_nodes; // 2D array of nodes (size is [dimensions.x][dimensions.y]

	Vector2i _dimensions; // rows and columns of nodes.
	Vector2f _cellSize;   // size of a cell in width and height

	// Neighbor node1 to node2.
	void connectNodes(PathfindingGraphNode *node1, PathfindingGraphNode *node2);

public:
	// these are the default graph node costs.
	// they can be overwritten by values stored in the level's file.(SZ)
	static const int BLOCKED = -1;
	static const int OPEN = 1;
	static const int STAIRS = 5;

	// These are the actual data members used to assign costs. (SZ)
	int _blockedCost;
	int _openCost;
	int _stairsCost;

	PathfindingGrid();
	~PathfindingGrid();

	void reset();

	void setupNodes(const TMX::TMXMap &map);

	// Return the node at the given point (SZ)
	PathfindingGraphNode *getNodeAtPoint(Vector2f point);
	// Return the node at the given coordinates (SZ)
	PathfindingGraphNode *getNodeAtCoords(int x, int y) {
		return &_nodes[x][y];
	}

	Vector2i getDimensions() {
		return _dimensions;
	}

	Vector2f getCellSize() {
		return _cellSize;
	}

	// Returns the nearest open node to the compare spot, starting with the given nodePos
	// and iterating through its neighbors. (SZ)
	PathfindingGraphNode *getNearestOpenNode(Vector2f nodePos, Vector2f comparePos);

	// Return true if two nodes share and adjacency to the same blocked node.
	// Can be used to find corners that shouldn't be cut.
	Common::Array<PathfindingGraphNode *> cornerCheck(const PathfindingGraphNode *node1, const PathfindingGraphNode *node2);
};

} // End of namespace Crab

#endif // CRAB_PATHFINDINGGRID_H
