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

#include "common/algorithm.h"
#include "mm/mm1/data/character.h"
#include "mm/mm1/mm1.h"

namespace MM {
namespace MM1 {

static const int CLASS_HP_PER_LEVEL[6] = {
	12, 10, 10, 8, 6, 8
};

Arr58::Arr58() {
	for (int i = 0; i < 8; ++i)
		_arr[i].clear();
}

void Inventory::clear() {
	_items.clear();
	_items.resize(INVENTORY_COUNT);
}

void Inventory::synchronize(Common::Serializer &s, bool ids) {
	for (int i = 0; i < INVENTORY_COUNT; ++i)
		s.syncAsByte(ids ? _items[i]._id : _items[i]._charges);
}

bool Inventory::empty() const {
	for (uint i = 0; i < INVENTORY_COUNT; ++i) {
		if (_items[i])
			return false;
	}
	return true;
}

bool Inventory::full() const {
	for (uint i = 0; i < INVENTORY_COUNT; ++i) {
		if (!_items[i])
			return false;
	}
	return true;
}

uint Inventory::size() const {
	for (int i = INVENTORY_COUNT - 1; i >= 0; --i) {
		if (_items[i])
			return i + 1;
	}
	return 0;
}

uint Inventory::add(byte id, byte charges) {
	uint idx = getFreeSlot();
	_items[idx]._id = id;
	_items[idx]._charges = charges;

	return idx;
}

int Inventory::getFreeSlot() const {
	for (uint i = 0; i < INVENTORY_COUNT; ++i) {
		if (!_items[i])
			return i;
	}

	error("Inventory is full");
	return -1;
}


void Inventory::removeAt(uint idx) {
	_items.remove_at(idx);
	_items.push_back(Entry());
}

bool Inventory::hasCategory(CategoryFn fn) const {
	for (uint i = 0; i < INVENTORY_COUNT; ++i) {
		if (fn(_items[i]._id))
			return true;
	}
	return false;
}

/*------------------------------------------------------------------------*/

void Character::synchronize(Common::Serializer &s) {
	s.syncBytes((byte *)_name, 16);
	s.syncAsByte(_sex);
	s.syncAsByte(_alignmentInitial);
	s.syncAsByte(_alignment);
	s.syncAsByte(_race);
	s.syncAsByte(_class);

	_intelligence.synchronize(s);
	_might.synchronize(s);
	_personality.synchronize(s);
	_endurance.synchronize(s);
	_speed.synchronize(s);
	_accuracy.synchronize(s);
	_luck.synchronize(s);
	_level.synchronize(s);
	_age.synchronize(s);

	s.syncAsUint32LE(_exp);
	s.syncAsUint16LE(_sp._current);
	s.syncAsUint16LE(_sp._base);
	_spellLevel.synchronize(s);

	s.syncAsUint16LE(_gems);
	s.syncAsUint16LE(_hpBase);
	s.syncAsUint16LE(_hp);
	s.syncAsUint16LE(_hpMax);

	// Gold field is annoying by being 3 bytes
	uint goldLo = _gold & 0xffff;
	uint goldHi = _gold >> 16;
	s.syncAsUint16LE(goldLo);
	s.syncAsByte(goldHi);
	if (s.isLoading())
		_gold = goldLo | (goldHi << 16);

	_ac.synchronize(s);
	s.syncAsByte(_food);
	s.syncAsByte(_condition);

	_equipped.synchronize(s, true);
	_backpack.synchronize(s, true);
	_equipped.synchronize(s, false);
	_backpack.synchronize(s, false);

	// TODO: Figure purpose of remaining unknown fields
	s.skip(39);
}

void Character::clear() {
	Common::fill(_name, _name + 16, 0);
	_sex = (Sex)0;
	_alignmentInitial = (Alignment)0;
	_alignment = (Alignment)0;
	_race = (Race)0;
	_class = (CharacterClass)0;
	_intelligence = _might = _personality = _endurance = 0;
	_speed = _accuracy = _luck = 0;
	_level = 0;
	_age = 0;
	_exp = 0;
	_sp = 0;
	_spellLevel = 0;
	_gems = 0;
	_hpBase = _hp = _hpMax = 0;
	_gold = 0;
	_ac = 0;
	_food = 0;
	_condition = 0;
	_quest = 0;
	_equipped.clear();
	_backpack.clear();

	_alignmentInitial = GOOD;
	_alignment = GOOD;
	_arr58._s._v58.clear();
	_arr58._s._v62.clear();
	_arr58._s._v64.clear();
	_arr58._s._v66.clear();
	_v6c = _alignmentCtr = 0;
}


void Character::gatherGold() {
	uint total = 0;
	for (uint i = 0; i < g_globals->_party.size(); ++i) {
		total += g_globals->_party[i]._gold;
		g_globals->_party[i]._gold = 0;
	}

	_gold = total;
}

Character::TradeResult Character::trade(int whoTo, int itemIndex) {
	Character &dest = g_globals->_party[whoTo];
	if (&dest == this)
		return TRADE_SUCCESS;

	if (dest._backpack.full())
		return TRADE_FULL;
	if (!_backpack[itemIndex])
		return TRADE_NO_ITEM;

	Inventory::Entry e = _backpack[itemIndex];
	_backpack.removeAt(itemIndex);
	dest._backpack.add(e._id, e._charges);

	return TRADE_SUCCESS;
}

Character::LevelIncrease Character::increaseLevel() {
	++_level;
	++_age;
	if (_age._base > 220)
		_age._base = 220;
	_v6c += 2;

	int classNum = _class == NONE ? ROBBER : _class;
	int newHP = g_engine->getRandomNumber(CLASS_HP_PER_LEVEL[classNum - 1]);

	if (_endurance._base >= 40)
		newHP += 10;
	else if (_endurance._base >= 35)
		newHP += 9;
	else if (_endurance._base >= 30)
		newHP += 8;
	else if (_endurance._base >= 27)
		newHP += 7;
	else if (_endurance._base >= 24)
		newHP += 6;
	else if (_endurance._base >= 21)
		newHP += 5;
	else if (_endurance._base >= 19)
		newHP += 4;
	else if (_endurance._base >= 17)
		newHP += 3;
	else if (_endurance._base >= 15)
		newHP += 2;
	else if (_endurance._base >= 13)
		newHP += 1;
	else if (_endurance._base >= 9)
		newHP += 0;
	else if (_endurance._base >= 7)
		newHP = MAX(newHP - 1, 1);
	else if (_endurance._base >= 5)
		newHP = MAX(newHP - 2, 1);
	else
		newHP = MAX(newHP - 3, 1);

	_hpBase += newHP;
	_hp = _hpMax = _hpBase;

	int gainedSpells = 0;
	if (classNum < ARCHER) {
		if (_level._base < 7)
			gainedSpells = 0;
		else if (_level._base == 7)
			gainedSpells = 1;
		else if (_level._base == 9)
			gainedSpells = 2;
		else if (_level._base == 11)
			gainedSpells = 3;
		else if (_level._base == 13)
			gainedSpells = 4;
	} else if (classNum < SORCERER) {
		if (_level._base == 3)
			gainedSpells = 2;
		else if (_level._base == 5)
			gainedSpells = 3;
		else if (_level._base == 7)
			gainedSpells = 4;
		else if (_level._base == 9)
			gainedSpells = 5;
		else if (_level._base == 11)
			gainedSpells = 6;
		else if (_level._base == 13)
			gainedSpells = 7;
	}

	LevelIncrease result;
	result._numHP = newHP;
	result._numSpells = gainedSpells;
	return result;
}

Character::BuyResult Character::buyItem(byte itemId) {
	getItem(itemId);

	// Check if backpack is full
	int slotIndex = 0;
	while (slotIndex < INVENTORY_COUNT && _backpack[slotIndex])
		++slotIndex;
	if (slotIndex == INVENTORY_COUNT)
		return BUY_BACKPACK_FULL;

	// Check character has enough gold
	Item &item = g_globals->_currItem;
	if (_gold < item._cost)
		return BUY_NOT_ENOUGH_GOLD;

	// Add the item
	_gold -= item._cost;
	_backpack.add(itemId, item._maxCharges);

	return BUY_SUCCESS;
}

void Character::updateAttributes() {
	_intelligence.reset();
	_might.reset();
	_personality.reset();
	_endurance.reset();
	_speed.reset();
	_personality.reset();
	_endurance.reset();
	_speed.reset();
	_accuracy.reset();
	_luck.reset();
	_level.reset();
	_spellLevel.reset();
}

void Character::updateAC() {
	int ac = _ac._base;

	if (_speed >= 40)
		ac += 9;
	else if (_speed >= 35)
		ac += 8;
	else if (_speed >= 30)
		ac += 7;
	else if (_speed >= 25)
		ac += 6;
	else if (_speed >= 21)
		ac += 5;
	else if (_speed >= 19)
		ac += 4;
	else if (_speed >= 17)
		ac += 3;
	else if (_speed >= 15)
		ac += 2;
	else if (_speed >= 13)
		ac += 1;
	else if (_speed >= 9)
		ac += 0;
	else if (_speed >= 7)
		ac = MAX(ac - 1, 0);
	else if (_speed >= 5)
		ac = MAX(ac - 2, 0);
	else
		ac = MAX(ac - 3, 0);

	_ac._current = ac;
}

void Character::updateSP() {
	int intelligence = _intelligence._current;
	int personality = _personality._current;
	int level = _level._current;
	int index = 3;
	AttributePair newSP;

	// Spell points only relevant for spell casters
	if (_spellLevel._current) {
		int threshold = -1;
		if (_class == CLERIC)
			threshold = personality;
		else if (_class == SORCERER)
			threshold = intelligence;
		else if (level < 7)
			threshold = -1;
		else {
			level -= 6;
			threshold = (_class == PALADIN) ?
				personality : intelligence;
		}

		if (threshold >= 40)
			index += 10;
		else if (threshold >= 35)
			index += 9;
		else if (threshold >= 30)
			index += 8;
		else if (threshold >= 27)
			index += 7;
		else if (threshold >= 24)
			index += 6;
		else if (threshold >= 21)
			index += 5;
		else if (threshold >= 19)
			index += 4;
		else if (threshold >= 17)
			index += 3;
		else if (threshold >= 15)
			index += 2;
		else if (threshold >= 13)
			index += 1;
		else if (threshold < 5)
			index -= 3;
		else if (threshold < 7)
			index -= 2;
		else if (threshold < 9)
			index -= 1;

		// Calculate the SP
		newSP._base += index * level;
		newSP._current = newSP._base;
	}

	// Set the character's new SP
	_sp = newSP;
}

void Character::update58() {
	for (int i = 0; i < 8; ++i)
		_arr58._arr[i]._current = _arr58._arr[i]._base;
}

Common::String Character::getConditionString() const {
	Common::String result;
	int cond = _condition;

	if (cond == 0) {
		result += STRING["stats.conditions.good"];
	} else if (cond == ERADICATED) {
		result += STRING["stats.conditions.eradicated"];
	} else {
		if (cond & BAD_CONDITION) {
			// Fatal conditions
			if (cond & DEAD)
				result += STRING["stats.conditions.dead"];
			if (cond & STONE)
				result += STRING["stats.conditions.stone"];
		} else {
			if (cond & UNCONSCIOUS)
				result += STRING["stats.conditions.unconscious"];
			if (cond & PARALYZED)
				result += STRING["stats.conditions.paralyzed"];
			if (cond & POISONED)
				result += STRING["stats.conditions.poisoned"];
			if (cond & DISEASED)
				result += STRING["stats.conditions.diseased"];
			if (cond & SILENCED)
				result += STRING["stats.conditions.silenced"];
			if (cond & BLINDED)
				result += STRING["stats.conditions.blinded"];
			if (cond & ASLEEP)
				result += STRING["stats.conditions.asleep"];
		}

		result.deleteLastChar();
	}

	return result;
}

void Character::castUnknown() {
	warning("TODO: castUnknown method");
}

void Character::rest() {
	// Characters with a bad condition like
	// being stoned can't rest
	if (_condition & BAD_CONDITION)
		return;

	updateSP();
	updateAttributes();
	updateAC();
	update58();

	_condition &= ~(ASLEEP | BLINDED | SILENCED |
		PARALYZED | UNCONSCIOUS);

	if (_hpBase == 0)
		_hpBase = 1;

	if (_age._current++ == 255) {
		_age._base = MIN((int)_age._base + 1, 255);
	}
	if ((g_engine->getRandomNumber(70) + 80) < _age._base) {
		_condition = UNCONSCIOUS | BAD_CONDITION;
		return;
	}

	if (_age._base >= 60) {
		// Fun fact: in the original if any of the attributes
		// reach zero, then it jumps to an instruction that
		// jumps to itself, freezing the game.
		if (--_might._current == 0 ||
			--_endurance._current == 0 ||
			--_speed._current == 0)
			error("Attributes bottomed out during rest");

		if (_age._base >= 70) {
			if (--_might._current == 0 ||
				--_endurance._current == 0 ||
				--_speed._current == 0)
				error("Attributes bottomed out during rest");
		}

		if (_age._base >= 80) {
			if (_might._current <= 2)
				error("Attributes bottomed out during rest");
			_might._current -= 2;
		}
	}

	if (_food > 0) {
		--_food;

		if (_condition & POISONED) {
			_hpMax /= 2;
		} else {
			_hpMax = _hp;
		}

		if (_condition & DISEASED) {
			_hpBase = _hpMax;
			_sp._current = _sp._base;
		}
	}
}

} // namespace MM1
} // namespace MM