/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      divsm.cpp
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

#include "divsm.hpp"

namespace SDF::Bignum::Primitives
{
	TwoDigit divBySmall(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		unsigned int smallNum, std::size_t len, TwoDigit remainder)
	{
		for (std::size_t i(len); i > 0; --i) {
			remainder = (remainder * BASE) + a[i - 1]; // "digit down" step
			TwoDigit quotient(remainder / smallNum); // divide by divisor to get 1 quotient digit
			remainder -= quotient * smallNum;
			r[i - 1] = static_cast<Digit>(quotient);
		}

		return remainder;
	}

	std::size_t fpDivBySmall(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		unsigned int smallNum, std::size_t rLen, std::size_t len)
	{
		TwoDigit remainder(0);
		std::size_t i(len);
		std::size_t digitsRemaining(rLen);
		std::size_t numLeadingZeroes(0);
		bool outputting(false);
		while(digitsRemaining) {
			if(i > 0) {
				remainder = (remainder * BASE) + a[i - 1]; // "digit down" step
			} else {
				remainder = remainder * BASE;
			}

			TwoDigit quotient(remainder / smallNum);// divide by divisor to get 1 quotient digit
			remainder -= quotient * smallNum;
			if(quotient != 0) {
				outputting = true;
			}

			if(outputting) {
				r[digitsRemaining - 1] = static_cast<Digit>(quotient);
				--digitsRemaining;
			} else {
				++numLeadingZeroes;
			}

			if(i > 0) {
				--i;
			}
		}

		return numLeadingZeroes;
	}

	TwoDigit modBySmall(Memory::SafePtr<const Digit> a, unsigned int smallNum, std::size_t len)
	{
		TwoDigit remainder(0);
		for (std::size_t i(len); i > 0; --i) {
			remainder = (remainder * BASE) + a[i - 1];
			remainder %= smallNum;
		}

		return remainder;
	}
}
