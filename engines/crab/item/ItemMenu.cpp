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

#include "crab/item/ItemMenu.h"

namespace Crab {

using namespace pyrodactyl::ui;
using namespace pyrodactyl::item;
using namespace pyrodactyl::people;

//------------------------------------------------------------------------
// Purpose: Load the layout and items
//------------------------------------------------------------------------
void ItemMenu::init(const ItemSlot &ref, const Vector2i &inc, const uint &rows, const uint &cols,
					const bool &keyboard) {
	uint size = rows * cols;
	for (uint i = 0; i < size; ++i) {
		ItemSlot b;
		b.init(ref, inc.x * (i % cols), inc.y * (i / cols));
		_element.push_back(b);
	}

	_useKeyboard = keyboard;
	assignPaths();
}

//------------------------------------------------------------------------
// Purpose: Load items from file
//------------------------------------------------------------------------
void ItemMenu::loadState(rapidxml::xml_node<char> *node) {
	uint count = 0;
	for (auto n = node->first_node(); n != nullptr && count < _element.size(); n = n->next_sibling(), ++count)
		_element[count].loadState(n);
}

//------------------------------------------------------------------------
// Purpose: Save to file
//------------------------------------------------------------------------
void ItemMenu::saveState(rapidxml::xml_document<> &doc, rapidxml::xml_node<char> *root) {
	for (auto &i : _element)
		i.saveState(doc, root);
}

//------------------------------------------------------------------------
// Purpose: Handles drag n' drop - return type is Boolean because we only need to communicate stat changes
//------------------------------------------------------------------------
void ItemMenu::handleEvents(const Common::Event &event, const int &xOffset, const int &yOffset) {
	int result = Menu<ItemSlot>::handleEvents(event);
	if (result != -1) {
		_selectIndex = result;
		for (uint i = 0; i < _element.size(); ++i)
			_element[i].state(i == (uint)_selectIndex);
	}
}

//------------------------------------------------------------------------
// Purpose: Draw the slot backgrounds first, then the items
//------------------------------------------------------------------------
void ItemMenu::draw(ItemDesc &itemInfo) {
	if (_selectIndex != -1)
		itemInfo.draw(_element[_selectIndex]._item);

	for (auto &i : _element)
		i.draw();
}

//------------------------------------------------------------------------
// Purpose: Equip an item at the first available location (used in events)
//------------------------------------------------------------------------
bool ItemMenu::equip(Item &item) {
	for (auto &i : _element)
		if (i._category == SLOT_STORAGE && i.equip(item))
			return true;

	return false;
}

//------------------------------------------------------------------------
// Purpose: Remove all instances of an item
//------------------------------------------------------------------------
bool ItemMenu::del(const Common::String &id) {
	bool result = false;

	for (auto &i : _element)
		if (i._item._id == id) {
			i._empty = true;
			i._item.clear();
			i._unread = false;
			result = true;
			break;
		}

	return result;
}

//------------------------------------------------------------------------
// Purpose: Find out if we have an item with a name
//------------------------------------------------------------------------
bool ItemMenu::has(const Common::String &container, const Common::String &id) {
	for (const auto &i : _element)
		if (i._item._id == id) {
			if (container == "equip") {
				if (i._category == SLOT_EQUIP)
					return true;
			} else if (container == "storage") {
				if (i._category == SLOT_STORAGE)
					return true;
			} else
				return true;
		}

	return false;
}

//------------------------------------------------------------------------
// Purpose: Swap an item with another item menu
//------------------------------------------------------------------------
bool ItemMenu::swap(ItemMenu &target, int index) {
	// We need to scan the slots first for an empty slot to store the item.
	// If no empty slot is found, then swap with a filled slot of same type
	// If no slot of type is found, don't swap items at all
	int foundIndex = -1, curIndex = 0;

	for (auto i = _element.begin(); i != _element.end(); ++i, ++curIndex)
		if (i->_itemType == target._element[index]._itemType) {
			if (i->_empty) {
				i->swap(target._element[index]);
				return true;
			} else
				foundIndex = curIndex;
		}

	if (foundIndex != -1) {
		_element[foundIndex].swap(target._element[index]);
		return true;
	}

	return false;
}

} // End of namespace Crab
