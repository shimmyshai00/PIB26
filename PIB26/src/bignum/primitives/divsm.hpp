/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      divsm.hpp
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

#ifndef SRC_BIGNUM_PRIMITIVES_DIVSM_HPP_
#define SRC_BIGNUM_PRIMITIVES_DIVSM_HPP_

#include "../../memory/SafePtr.hpp"

#include "../defs.hpp"

#include <cstddef>

namespace SDF::Bignum::Primitives
{
	// Function:  divBySmall
	// Purpose:   Divide digits in a buffer by a small number. Also works in-place.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into a buffer holding the digit string to divide.
	//            smallNum - The small number to divide by.
	//            len - The number of digits to divide.
	//            remainder - Incoming remainder.
	// Returns:   The outgoing remainder.
	TwoDigit divBySmall(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		unsigned int smallNum, std::size_t len, TwoDigit remainder);

	// Function:  fpDivBySmall
	// Purpose:   Divide digits in a buffer by a small number in a "normalizing" or "floating point"
	//            fashion, i.e. we omit the leading zeroes and fill out the full precision of the
	//            result buffer with the quotient.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into a buffer holding the digit string to divide.
	//            smallNum - The small number to divide by.
	//            rLen - The length of the result to return.
	//            len - The number of digits to divide.
	// Returns:   The number of leading zeroes ignored.
	std::size_t fpDivBySmall(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		unsigned int smallNum, std::size_t rLen, std::size_t len);

	// Function:  modBySmall
	// Purpose:   Modulo digits in a buffer by a small number.
	// Arguments: a - A pointer into a buffer holding the digit string to modulo.
	//            smallNum - The small number to modulo by.
	//            len - The number of digits to modulo.
	// Returns:   The result of a % smallNum.
	TwoDigit modBySmall(Memory::SafePtr<const Digit> a, unsigned int smallNum, std::size_t len);

}

#endif /* SRC_BIGNUM_PRIMITIVES_DIVSM_HPP_ */
