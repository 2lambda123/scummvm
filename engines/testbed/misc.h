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

#ifndef TESTBED_MISC_H
#define TESTBED_MISC_H

#include "testbed/testsuite.h"

#include "common/mutex.h"


namespace Testbed {

// Shared variables used in mutex handling test
struct SharedVars {
	int first;
	int second;
	bool resultSoFar;
	Common::Mutex *mutex;
};

namespace MiscTests {

// Miscellaneous tests include testing datetime, timers and mutexes

// Helper functions for Misc tests
Common::String getHumanReadableFormat(const TimeDate &td);
void timerCallback(void *arg);
void criticalSection(void *arg);

// will contain function declarations for Misc tests
TestExitStatus testDateTime();
TestExitStatus testTimers();
TestExitStatus testMutexes();
TestExitStatus testOpenUrl();
TestExitStatus testImageAlbum();
// add more here

} // End of namespace MiscTests

class MiscTestSuite : public Testsuite {
public:
	/**
	 * The constructor for the MiscTestSuite
	 * For every test to be executed one must:
	 * 1) Create a function that would invoke the test
	 * 2) Add that test to list by executing addTest()
	 *
	 * @see addTest()
	 */
	MiscTestSuite();
	~MiscTestSuite() override {}
	const char *getName() const override {
		return "Misc";
	}
	const char *getDescription() const override {
		return "Miscellaneous: Timers/Mutexes/Datetime/openUrl/ImageAlbum";
	}
};

} // End of namespace Testbed

#endif // TESTBED_MISC_H
