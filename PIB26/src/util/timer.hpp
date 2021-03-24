/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      timer.hpp
 * Timestamp: Mar 20, 2021
 *
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SRC_UTIL_TIMER_HPP_
#define SRC_UTIL_TIMER_HPP_

#include <time.h>

namespace SDF::Util {
	// Function:   timeDiffMillis
	// Purpose:    Get the time difference between two high-resolution system times in
	//             milliseconds.
	// Parameters: endTime - The time at the end of the interval.
	//             startTime - The time at the start of the interval.
	// Returns:    endTime - startTime in milliseconds.
	unsigned long timeDiffMillis(struct timespec endTime, struct timespec startTime);
}

#endif /* SRC_UTIL_TIMER_HPP_ */
