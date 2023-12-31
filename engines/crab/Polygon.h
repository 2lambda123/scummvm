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

#ifndef CRAB_POLYGON_H
#define CRAB_POLYGON_H

#include "crab/Rectangle.h"
#include "crab/vectors.h"

namespace Crab {

//------------------------------------------------------------------------
// Purpose: The result of a collision
//------------------------------------------------------------------------
struct PolygonCollisionResult {
	// Are the Polygons currently intersecting?
	bool _intersect;

	// The translation to apply to polygon A to push the polygons apart
	// Also known as the minimum translation vector
	Vector2f _mtv;

	PolygonCollisionResult() {
		_intersect = false;
	}
};

//------------------------------------------------------------------------
// Purpose: A simple 2D Polygon class
//------------------------------------------------------------------------
class Polygon2D {
	void addPoint(const Vector2f &ref, const Common::String &x, const Common::String &y, Vector2f &min, Vector2f &max);

public:
	// A list of all points
	Common::Array<Vector2f> _point;

	// A list of all edges
	Common::Array<Vector2f> _edge;

	Polygon2D() {}
	Polygon2D(rapidxml::xml_node<char> *node, Rect &bounds) {
		load(node, bounds);
	}

	// Returns the approximate axis aligned bounding box of the polygon
	void load(rapidxml::xml_node<char> *node, Rect &bounds);

	void setEdge();

	Vector2f center() const;
	void offset(const float &x, const float &y);
	void offset(const Vector2f &v) {
		offset(v.x, v.y);
	}

	// Check if Polygon2D A is going to collide with Polygon2D B for the given velocity
	// PolyA is this polygon
	PolygonCollisionResult collide(const Polygon2D &polyB) const;

	// Code for collision with a rectangle
	PolygonCollisionResult collide(const Rect &rect) const;

	// Find if a point is inside this polygon
	bool contains(const float &x, const float &y);

	// Calculate the projection of a polygon on an axis and returns it as a [min, max] interval
	void project(const Vector2f &axis, float &min, float &max) const;

	void draw(const int &xOffset = 0, const int &yOffset = 0,
			  const uint8 &r = 0, const uint8 &g = 0, const uint8 &b = 0.0f, const uint8 &a = 255);
};

} // End of namespace Crab

#endif // CRAB_POLYGON_H
