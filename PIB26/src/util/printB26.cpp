/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      printB26.cpp
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

#include "printB26.hpp"

#include <algorithm>

namespace SDF::Util {
	std::string printB26(int num, std::size_t padding) {
		// Use C-style string for efficiency.
		char outStr[1024];
		std::size_t outLen(0);
		int sign;

		// Perform the base conversion.
		if(num < 0) {
			num = -num;
			sign = -1;
		} else {
			sign = +1;
		}

		while(num) {
			outStr[outLen++] = 'A' + (num % 26);
			num /= 26;
		}

		// Pad if necessary.
		if(outLen < padding) {
			for(std::size_t i(outLen); i < padding; ++i) {
				outStr[i] = 'A';
			}

			outLen = padding;
		}

		// Add sign if needed.
		if(sign < 0) {
			outStr[outLen++] = '-';
		}

		// Null-terminate for safety and reverse-convert to C++ string.
		outStr[outLen] = '\0';

		std::string rvString;
		for(std::size_t i(outLen); i > 0; --i) {
			rvString += outStr[i-1];
		}

		return std::string(rvString);
	}

	char printB26Digit(int dig) {
		return 'A' + dig;
	}
}

