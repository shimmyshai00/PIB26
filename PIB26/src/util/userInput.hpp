/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      userInput.hpp
 * Timestamp: Mar 24, 2021
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

#ifndef SRC_UTIL_USERINPUT_HPP_
#define SRC_UTIL_USERINPUT_HPP_

#include <string>

namespace SDF::Util {
	// Function:   getUserYNInput
	// Purpose:    Get the user's response to a "yes/no" question.
	// Parameters: prompt - The prompt to use (excluding the "yes/no" and question mark bits).
	// Returns:    A Boolean indicating the received response ("yes" = "true").
	bool getUserYNInput(std::string prompt);

	// Function:   getUserNumericInput
	// Purpose:    Get a numeric input from the user.
	// Parameters: prompt - The prompt to use (excluding the question mark bits).
	//             min - The minimum of the allowable range.
	//             max - The maximum of the allowable range.
	// Returns:    The value entered.
	int getUserNumericInput(std::string prompt, int min, int max);
}

#endif /* SRC_UTIL_USERINPUT_HPP_ */
