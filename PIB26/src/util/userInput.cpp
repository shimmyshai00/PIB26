/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      userInput.cpp
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

#include "userInput.hpp"

#include <iostream>

namespace SDF::Util
{
	bool getUserYNInput(std::string prompt)
	{
		std::string input;

		do {
			std::cout << prompt << " (Y/N)? ";
			std::getline(std::cin, input);
		} while ((input != "Y") && (input != "y") && (input != "N") && (input != "n"));

		if((input == "Y") || (input == "y"))
			return true;
		else
			return false;
	}

	int getUserNumericInput(std::string prompt, int min, int max)
	{
		int val;

		bool valid(true);
		do {
			try {
				std::string input;
				std::cout << prompt << " (" << min << " - " << max << ")" << ": ";
				std::getline(std::cin, input);
				val = std::stoi(input);

				if((val < min) || (val > max)) {
					valid = false;
				}
			} catch (std::exception &e) {
				valid = false;
			}
		} while (!valid);

		return val;
	}
}
