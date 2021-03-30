/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      printB26.hpp
 * Timestamp: Mar 28, 2021
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

#ifndef SRC_UTIL_PRINTB26_HPP_
#define SRC_UTIL_PRINTB26_HPP_

#include <string>
#include <cstddef>

namespace SDF::Util {
	// Function:   printB26
	// Purpose:    Print a number in base-26.
	// Parameters: num - The number to print.
	//             padding - The amount of padding, if any, to apply.
	// Returns:    A string representation of the number, in base-26 (A-Z).
	std::string printB26(int num, std::size_t padding);

	// Function:   printB26Digit
	// Purpose:    Print a single base-26 digit.
	// Parameters: dig - The digit to print.
	// Returns:    The character representing this digit.
	char printB26Digit(int dig);
}

#endif /* SRC_UTIL_PRINTB26_HPP_ */
