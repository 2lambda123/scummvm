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

#ifndef TESTBED_EVENTS_H
#define TESTBED_EVENTS_H

#include "testbed/testsuite.h"

namespace Testbed {

namespace EventTests {

// Helper functions for Event tests
char keystrokeToChar();
Common::Rect drawFinishZone();
// will contain function declarations for Event tests
TestExitStatus mouseEvents();
TestExitStatus doubleClickTime();
TestExitStatus kbdEvents();
TestExitStatus showMainMenu();
// add more here

} // End of namespace EventTests

class EventTestSuite : public Testsuite {
public:
	/**
	 * The constructor for the EventTestSuite
	 * For every test to be executed one must:
	 * 1) Create a function that would invoke the test
	 * 2) Add that test to list by executing addTest()
	 *
	 * @see addTest()
	 */
	EventTestSuite();
	~EventTestSuite() override {}
	const char *getName() const override {
		return "Events";
	}
	const char *getDescription() const override {
		return "Events : Keyboard/Mouse/Return to Launcher";
	}
};

} // End of namespace Testbed

#endif // TESTBED_EVENTS_H
