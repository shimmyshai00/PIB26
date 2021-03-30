/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      ComplexFft.cpp
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

#include "ComplexFft.hpp"
#include <cmath>

namespace SDF::Bignum::Multiplication::Fft::Complex {
	ComplexFft::ComplexFft() {}

	std::size_t ComplexFft::getMaxNumLengthAtBase(long base) const {
		// The size of the largest element in the multiplication pyramid, which has the greatest
		// chance to overflow the element numeric data type, is given by
		//
		//        Size in bits = 1 + lg(Factor size) + lg((BASE-1)^2)
		//
		// and this largest element occurs when multiplying all nines by all nines, i.e. a number
		// of the form (BASE^N - 1) with itself, where N is the factor size.
		//
		// What we want is the largest factor size, which is:
		//
		//        Factor size = 2^(Size in bits - 1 - lg((BASE-1)^2)).
		//
		// For example, with a bit size of 53 bits (i.e. an IEEE 754 double), and BASE of 10000,
		// we have a max size of a bit more than 9M elements, or 36M decimal digits. For a BASE of
		// 456976 (26^4), i.e. macro-base 26 with 4 base-26 digits per Digit, we have a max size of
		// 43,132 or 172,530 base-26 digits. If we want to go larger than these, we must use smaller
		// bases.

		// This actually underestimates a bit, which hopefully should compensate for other errors.
		std::size_t log2Base = ceil(log(base*base-1)/log(2.0));
		return 1 << (53 - log2Base); // 53 bits per double.
	}
}

