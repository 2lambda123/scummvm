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

#include "crab/crab.h"
#include "crab/item/ItemSlot.h"

namespace Crab {

using namespace pyrodactyl::ui;
using namespace pyrodactyl::item;
using namespace pyrodactyl::input;
using namespace pyrodactyl::music;
using namespace pyrodactyl::people;

//------------------------------------------------------------------------
// Purpose: Load
//------------------------------------------------------------------------
void ItemSlot::load(rapidxml::xml_node<char> *node) {
	StateButton::load(node);

	if (node->first_attribute("slot") == nullptr)
		_noType = true;
	else {
		loadStr(_itemType, "slot", node);
		_noType = false;
	}

	Common::String name = node->name();
	if (name == "equip")
		_category = SLOT_EQUIP;
	else
		_category = SLOT_STORAGE;
}

//------------------------------------------------------------------------
// Purpose: Initialize from reference item slot
//------------------------------------------------------------------------
void ItemSlot::init(const ItemSlot &ref, const int &xOffset, const int &YOffset) {
	StateButton::init(ref, xOffset, YOffset);
	_canmove = ref._canmove;
	_noType = ref._noType;
	_category = ref._category;
	_unread = ref._unread;
}

//------------------------------------------------------------------------
// Purpose: Save state to file
//------------------------------------------------------------------------
void ItemSlot::saveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root) {
	rapidxml::xml_node<char> *child;

	if (_category == SLOT_EQUIP)
		child = doc.allocate_node(rapidxml::node_element, "equip");
	else
		child = doc.allocate_node(rapidxml::node_element, "storage");

	_item.saveState(doc, child);
	saveBool(_unread, "unread", doc, child);

	root->append_node(child);
}

//------------------------------------------------------------------------
// Purpose: Load state from file
//------------------------------------------------------------------------
void ItemSlot::loadState(rapidxml::xml_node<char> *node) {
	_item.load(node);
	loadBool(_unread, "unread", node);

	if (_item._id == "")
		_empty = true;
	else
		_empty = false;
}

//------------------------------------------------------------------------
// Purpose: Draw
//------------------------------------------------------------------------
void ItemSlot::draw() {
	StateButton::draw();

	if (!_empty)
		_item.draw(x, y);

	if (_unread)
		g_engine->_imageManager->notifyDraw(x + w, y);
}

//------------------------------------------------------------------------
// Purpose: Handle user input
//------------------------------------------------------------------------
ButtonAction ItemSlot::handleEvents(const Common::Event &event, const int &xOffset, const int &yOffset) {
	ButtonAction ac = StateButton::handleEvents(event, xOffset, yOffset);
	if (ac == BUAC_LCLICK || ac == BUAC_RCLICK)
		_unread = false;

	return ac;
}

//------------------------------------------------------------------------
// Purpose: Exchange items with another slot
// this object is the current slot, parameter object is target slot
//------------------------------------------------------------------------
bool ItemSlot::swap(ItemSlot &target) {
	if (canSwap(target)) {
		Item temp = _item;
		_item = target._item;
		target._item = temp;

		bool val = _empty;
		_empty = target._empty;
		target._empty = val;

		return true;
	}

	return false;
}

//------------------------------------------------------------------------
// Purpose: Equip an item
//------------------------------------------------------------------------
bool ItemSlot::equip(Item &i) {
	if ((_itemType == i._type || _noType) && _empty) {
		_item = i;
		_empty = false;
		_unread = true;
		return true;
	}

	return false;
}

//------------------------------------------------------------------------
// Purpose: Change stats based on item
//------------------------------------------------------------------------
void ItemSlot::statChange(pyrodactyl::people::Person &obj, bool increase) {
	if (_enabled)
		_item.statChange(obj, increase);
}

} // End of namespace Crab
