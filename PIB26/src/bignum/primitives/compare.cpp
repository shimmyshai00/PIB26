/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      compare.cpp
 * Timestamp: Mar 16, 2021
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

#include "compare.hpp"

namespace SDF::Bignum::Primitives
{
	std::size_t countSignifDigits(Memory::SafePtr<const Digit> r, std::size_t len)
	{
		for (std::size_t i(len); i > 0; --i) {
			if (r[i - 1] != 0)
				return i;
		}

		return 0;
	}

	std::size_t countSignifDigits(TwoDigit smallNum) {
		std::size_t rv(0);

		while(smallNum) {
			smallNum /= BASE;
			++rv;
		}

		return rv;
	}

	bool testZero(Memory::SafePtr<const Digit> r, std::size_t len)
	{
		for (std::size_t i(0); i < len; ++i) {
			if (r[i] != 0)
				return false;
		}

		return true;
	}

	bool revTestZero(Memory::SafePtr<const Digit> r, std::size_t len)
	{
		for (std::size_t i(1); i < len; ++i) {
			if (*(r - i) != 0)
				return false;
		}

		return true;
	}

	int compare(Memory::SafePtr<const Digit> a, Memory::SafePtr<const Digit> b, std::size_t len)
	{
		for (std::size_t i(len); i > 0; --i) {
			if (a[i - 1] > b[i - 1])
				return +1;
			if (a[i - 1] < b[i - 1])
				return -1;
		}

		return 0;
	}

	int revCompare(Memory::SafePtr<const Digit> a, Memory::SafePtr<const Digit> b, std::size_t len)
	{
		for (std::size_t i(1); i < len; ++i) {
			if (*(a - i) > *(b - i))
				return +1;
			if (*(a - i) < *(b - i))
				return -1;
		}

		return 0;
	}
}
